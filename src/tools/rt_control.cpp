#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <win32_wrapper/init.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>
#include <win32_wrapper/layout/tiled_layout.hpp>

#include <camcon/preset_store.hpp>

#include <system/str_tools.hpp>

#include <Dbt.h>
#include <ks.h>
#include <ksmedia.h>
#include <stringapiset.h>
#include <windowsx.h>

#include <iostream>
#include <exception>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

const int W_WIDTH = 600;
const int W_HEIGHT = 300;
const std::wstring VERSION_STR {L"v3.3"};
const std::wstring TITLE {L"CAMCON"};
const int MAX_EDIT_TEXT {64};

HDEVNOTIFY registerForDeviceNotification(HWND hwnd);
fs::path getExePath();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    mfw::initialize();
    auto [gdiToken, gdiStartupInput] = win32w::init();

    int argc;
    auto argv {CommandLineToArgvW(pCmdLine, &argc)};

    if(argc > 1 || !system_u::str_tools::is_integer(argv[0]))
    {
        MessageBox(nullptr, L"Missing device index\n\nUsage:\n\trt-control <DEVICE_INDEX>", L"Error", MB_OK);
        // std::wcout << L"Missing device index\n\nUsage:\n\trt-control <DEVICE_INDEX>\n";
        return 1;
    }

    auto devIdx {std::stoi(argv[0])};

    mfw::VideoDeviceEnumerator vde{};

    try {
        vde.checkIndexInBounds(devIdx);
    } catch(std::exception e)
    {
        MessageBox(nullptr, L"There is no device with the provided index", L"Error", MB_OK);
        return 2;
    }

    // auto devSrc {vde.getDevice(devIdx)};
    auto devSymbLink {vde.getSymbolicLink(devIdx)};
    auto devSrc {vde.getDeviceFromSymbolicLink(devSymbLink)};
    mfw::CameraController cc{};
    cc.setDevice(devSrc);

    win32w::WindowBuilder wb{};
    wb.generateClassName();
    wb.createClass();
    auto style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    auto win_title = system_u::str_tools::join({TITLE, VERSION_STR, L"Connected"});
    auto win = wb.build(win_title, CW_USEDEFAULT, CW_USEDEFAULT, W_WIDTH, W_HEIGHT, style);

    int btn_size = 64;
    int center_x = (int) (W_WIDTH - btn_size)/2;
    int center_y = (int) (W_HEIGHT - btn_size)/2;
    int btn_padding = 10;

    win32w::TiledLayout tl{};
    tl.wItemSize = btn_size;
    tl.hItemSize = btn_size;
    tl.padding = btn_padding;
    tl.x = 20;
    tl.y = 20;

    win32w::ControlFactory cf{};
    // auto btnLeft = cf.createButton(win, L"<", (int) center_x - btn_padding - btn_size, center_y, btn_size, btn_size);
    // auto btnRight = cf.createButton(win, L">", (int) center_x + btn_padding + btn_size, center_y, btn_size, btn_size);
    // auto btnUp = cf.createButton(win, L"^", (int) center_x, center_y  - btn_padding - btn_size, btn_size, btn_size);
    // auto btnDown = cf.createButton(win, L"v", (int) center_x, center_y + btn_padding + btn_size, btn_size, btn_size);
    // auto btnIn = cf.createButton(win, L"+", (int) center_x + btn_padding + btn_size, center_y  - btn_padding - btn_size, btn_size, btn_size);
    // auto btnOut = cf.createButton(win, L"-", (int) center_x + btn_padding + btn_size, center_y  + btn_padding + btn_size, btn_size, btn_size);
    int x, y;
    std::tie(x, y) = tl.computePosition(0, 1);
    auto btnLeft = cf.createButton(win, L"<", x, y, btn_size, btn_size);
    std::tie(x, y) = tl.computePosition(2, 1);
    auto btnRight = cf.createButton(win, L">", x, y, btn_size, btn_size);
    std::tie(x, y) = tl.computePosition(1, 0);
    auto btnUp = cf.createButton(win, L"^", x, y, btn_size, btn_size);
    std::tie(x, y) = tl.computePosition(1, 2);
    auto btnDown = cf.createButton(win, L"v", x, y, btn_size, btn_size);
    std::tie(x, y) = tl.computePosition(2, 0);
    auto btnIn = cf.createButton(win, L"+", x, y, btn_size, btn_size);
    std::tie(x, y) = tl.computePosition(2, 2);
    auto btnOut = cf.createButton(win, L"-", x, y, btn_size, btn_size);

    auto cbPreset = cf.createComboBox(win, W_WIDTH/2 + 20, 10, W_WIDTH/2 - 40, 160);
    auto editSave = cf.createEdit(win, W_WIDTH/2 + 20, 48, W_WIDTH/2 - 40, 24);
    auto btnSave = cf.createButton(win, L"Save Preset", W_WIDTH - 20 - 160, 68 + 16, 160, 32);
    
    ComboBox_SetText(cbPreset->hwnd, L"Custom");
    Edit_LimitText(editSave->hwnd, MAX_EDIT_TEXT);
    
    camcon::PresetStore ps{};

    ps.root = getExePath();
    auto presets = ps.loadPresets();
    for(auto & p : presets)
    {
        ComboBox_AddString(cbPreset->hwnd, p.name.c_str());
    }


    btnLeft->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Pan);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };
    
    btnRight->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Pan);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };
    
    btnUp->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Tilt);
        // auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Exposure);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };
    
    btnDown->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Tilt);
        // auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Exposure);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };
    
    btnIn->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Zoom);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };
    
    btnOut->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Zoom);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };

    btnSave->onClick = [&](HWND hwnd) {
        if(!cc.deviceIsSet()) {
            return;
        }

        std::vector<mfw::CameraControlPropertyValue> ccprops {
            cc.getProperty(tagCameraControlProperty::CameraControl_Pan),
            cc.getProperty(tagCameraControlProperty::CameraControl_Tilt),
            cc.getProperty(tagCameraControlProperty::CameraControl_Zoom),
        };

        camcon::Preset p{};
        wchar_t tbuffer[MAX_EDIT_TEXT + 1] {};
        Edit_GetText(editSave->hwnd, tbuffer, MAX_EDIT_TEXT + 1);
        p.name = tbuffer;
        p.name = system_u::str_tools::trim(p.name);
        p.config = ccprops;

        bool found = false;
        int i;
        for(i = 0; i < presets.size(); i++)
        {
            auto& p2 { presets.at(i) };
            if(p2.name.compare(p.name) == 0)
            {
                p2.config = p.config;
                found = true;
                break;
            }
        }

        if(found) {
            ComboBox_SetCurSel(cbPreset->hwnd, i);
        } else {
            presets.emplace_back(p);
            ComboBox_AddString(cbPreset->hwnd, p.name.c_str());
            ComboBox_SetCurSel(cbPreset->hwnd, presets.size() - 1);
        }

        Edit_SetText(editSave->hwnd, L"");
        
        ps.savePreset(p);
    };

    cbPreset->onSelect = [&](HWND hwnd)
    {
        if(!cc.deviceIsSet()) {
            return;
        }

        auto currIdx = ComboBox_GetCurSel(cbPreset->hwnd);
        if(currIdx == CB_ERR)
        {
            return;
        }

        auto p = presets.at(currIdx);
        for(auto& prop : p.config)
        {
            cc.setProperty(prop.prop, prop.lvalue);
        }
    };

    auto windHdevNotify = registerForDeviceNotification(win->hwnd);
    win->setCallback(WM_DEVICECHANGE, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVICEARRIVAL)
        {
            auto dbh = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);

            if(dbh == nullptr || dbh->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE)
            {
                return;
            }
            
            auto dbdi = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(dbh);

            std::wstring rightSymbLink { dbdi->dbcc_name };

            auto llsl = system_u::str_tools::tolower(devSymbLink).substr(0, 50);
            auto lrsl = system_u::str_tools::tolower(rightSymbLink).substr(0, 50);

            if(llsl.compare(lrsl) == 0)
            {
                auto is_DBT_DEVICEREMOVECOMPLETE = wParam == DBT_DEVICEREMOVECOMPLETE;
                auto is_DBT_DEVICEARRIVAL = wParam == DBT_DEVICEARRIVAL;

                auto state = is_DBT_DEVICEREMOVECOMPLETE ? L"Disconnected" :
                             is_DBT_DEVICEARRIVAL ? L"Connected" : L"Unknow state";

                SetWindowText(hwnd, system_u::str_tools::join({TITLE, VERSION_STR, state}).c_str());

                if(is_DBT_DEVICEARRIVAL)
                {
                    cc.setDevice(vde.getDeviceFromSymbolicLink(devSymbLink));
                    Button_Enable(btnUp->hwnd, true);
                    Button_Enable(btnDown->hwnd, true);
                    Button_Enable(btnLeft->hwnd, true);
                    Button_Enable(btnRight->hwnd, true);
                    Button_Enable(btnIn->hwnd, true);
                    Button_Enable(btnOut->hwnd, true);
                    Button_Enable(btnSave->hwnd, true);
                    ComboBox_Enable(cbPreset->hwnd, true);
                    Edit_Enable(editSave->hwnd, true);
                } else {
                    cc.setDevice(nullptr);
                    Button_Enable(btnUp->hwnd, false);
                    Button_Enable(btnDown->hwnd, false);
                    Button_Enable(btnLeft->hwnd, false);
                    Button_Enable(btnRight->hwnd, false);
                    Button_Enable(btnIn->hwnd, false);
                    Button_Enable(btnOut->hwnd, false);
                    Button_Enable(btnSave->hwnd, false);
                    ComboBox_Enable(cbPreset->hwnd, false);
                    ComboBox_Enable(cbPreset->hwnd, false);
                    Edit_Enable(editSave->hwnd, false);
                }
            }
        } 
    });

    ShowWindow(win->hwnd, nCmdShow);
    
    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    windHdevNotify && UnregisterDeviceNotification(windHdevNotify);

    vde.release();

    mfw::finalize();
    win32w::finalize(gdiToken);
    return 0;
}

HDEVNOTIFY registerForDeviceNotification(HWND hwnd)
{
    DEV_BROADCAST_DEVICEINTERFACE di {0};
    di.dbcc_size = sizeof(di);
    di.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    di.dbcc_classguid = KSCATEGORY_CAPTURE;

    return RegisterDeviceNotification(hwnd, &di, DEVICE_NOTIFY_WINDOW_HANDLE);
}

fs::path getExePath()
{
    wchar_t path_buffer[MAX_PATH] {};

    GetModuleFileName(nullptr, path_buffer, MAX_PATH);

    return fs::path{path_buffer}.parent_path();
}