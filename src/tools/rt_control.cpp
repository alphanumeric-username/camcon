#include <camcon/constants.hpp>
#include <camcon/init.hpp>
#include <camcon/arg_parse.hpp>
#include <camcon/preset_store.hpp>
#include <camcon/utils.hpp>
#include <camcon/rt_control/select_device_window.hpp>

#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <win32_wrapper/init.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>
#include <win32_wrapper/layout/tiled_layout.hpp>
#include <win32_wrapper/gdi/gdi_text.hpp>

#include <system/str_tools.hpp>

#include <Dbt.h>
// #include <ks.h>
// #include <ksmedia.h>
#include <stringapiset.h>
#include <windowsx.h>

#include <algorithm>
#include <iostream>
#include <exception>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

camcon::arg_parse::RTControlArgs processArgs(PWSTR pCmdLine);
std::shared_ptr<win32w::Window> createMainWindow();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    camcon::init();

    auto args = processArgs(pCmdLine);
    
    auto devIdx = args.index;
    if(!args.indexWasProvided)
    {
        devIdx = camcon::promptDeviceIndexWindow();
        if(devIdx < 0)
        {
            return 0;
        }
    }

    mfw::VideoDeviceEnumerator vde{};

    try {
        vde.checkIndexInBounds(devIdx);
    } catch(std::exception e)
    {
        MessageBox(nullptr, L"There is no device with the provided index", L"Error", MB_OK);
        return 2;
    }

    auto devSymbLink {vde.getSymbolicLink(devIdx)};
    auto devSrc {vde.getDeviceFromSymbolicLink(devSymbLink)};
    mfw::CameraController cc{};
    cc.setDevice(devSrc);

    auto win = createMainWindow();

    int btn_size = 64;
    int btn_padding = 10;

    win32w::TiledLayout tl{};
    tl.itemWidth = btn_size;
    tl.itemHeight = btn_size;
    tl.padding = btn_padding;
    tl.x = 20;
    tl.y = 20;

    win32w::ControlFactory cf{};
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

    auto txtPreset = std::make_shared<win32w::GDIText>();
    auto cbPreset = cf.createComboBox(win, camcon::W_WIDTH/2 + 20, 10 + 16 + 10, camcon::W_WIDTH/2 - 50, 160);
    auto txtSave = std::make_shared<win32w::GDIText>();
    auto editSave = cf.createEdit(win, camcon::W_WIDTH/2 + 20, 10 + 16 + 10 + 16 + 10 + 16 + 20, camcon::W_WIDTH/2 - 50, 24);
    auto btnSave = cf.createButton(win, L"Save Preset", camcon::W_WIDTH - 30 - 160, 10 + 16 + 10 + 16 + 10 + 16 + 20 + 16 + 20, 160, 32);

    auto pr = cc.getPropertyRange(tagCameraControlProperty::CameraControl_Zoom);

    auto tbZoom = cf.createTrackBar(win, L"Zoom", camcon::W_WIDTH/2 - 40, 20, 40, camcon::W_HEIGHT - 80, pr.pMin, pr.pMax,
        WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_TRANSPARENTBKGND );

    txtPreset->contents = L"Current preset:";
    txtPreset->fontBuilder.size = 16;
    txtPreset->x = camcon::W_WIDTH/2 + 20;
    txtPreset->y = 10;

    txtSave->contents = L"New preset:";
    txtSave->fontBuilder.size = 16;
    txtSave->x = camcon::W_WIDTH/2 + 20;
    txtSave->y = 10 + 16 + 10 + 16 + 20;

    win->addGDIText(txtPreset);
    win->addGDIText(txtSave);
    
    // ComboBox_SetText(cbPreset->hwnd, );
    cbPreset->setText(L"Custom");
    Edit_LimitText(editSave->hwnd, camcon::MAX_EDIT_TEXT);
    
    camcon::PresetStore ps{};

    ps.root = camcon::getExePath();
    auto presets = ps.loadPresets();
    
    for(auto & p : presets)
    {
        cbPreset->addItem(p.name);
    }

    auto varyCamProp = [&](tagCameraControlProperty ccp, int dir)
    {
        auto prop = cc.getProperty(ccp);

        int sign = dir > 0 ? 1
                 : dir < 0 ? -1
                 : 0;

        auto delta = sign * (GetKeyState(VK_SHIFT) & 0x8000 ? 10 : 1);

        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + delta);
        }
        ComboBox_SetText(cbPreset->hwnd, L"Custom");
    };

    btnLeft->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Pan, -1);
    };
    
    btnRight->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Pan, 1);
    };
    
    btnUp->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Tilt, 1);
    };
    
    btnDown->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Tilt, -11);
    };
    
    btnIn->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Zoom, 1);
    };
    
    btnOut->onClick = [&](HWND hwnd) {
        varyCamProp(tagCameraControlProperty::CameraControl_Zoom, -1);
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
        p.name = sys::str_tools::trim(editSave->getText());

        if(p.name.empty())
        {
            return;
        }

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
            cbPreset->addItem(p.name);
            ComboBox_SetCurSel(cbPreset->hwnd, presets.size() - 1);
        }

        editSave->setText(L"");
        
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

    auto windHdevNotify = camcon::registerForDeviceNotification(win->hwnd);
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

            auto llsl = sys::str_tools::tolower(devSymbLink).substr(0, 50);
            auto lrsl = sys::str_tools::tolower(rightSymbLink).substr(0, 50);

            if(llsl.compare(lrsl) == 0)
            {
                auto is_DBT_DEVICEREMOVECOMPLETE = wParam == DBT_DEVICEREMOVECOMPLETE;
                auto is_DBT_DEVICEARRIVAL = wParam == DBT_DEVICEARRIVAL;

                auto state = is_DBT_DEVICEREMOVECOMPLETE ? L"Disconnected" :
                             is_DBT_DEVICEARRIVAL ? L"Connected" : L"Unknow state";

                SetWindowText(hwnd, sys::str_tools::join({camcon::TITLE, camcon::VERSION_STR, state}).c_str());

                if(is_DBT_DEVICEARRIVAL)
                {
                    cc.setDevice(vde.getDeviceFromSymbolicLink(devSymbLink));
                    win->enableControls(true);
                } else {
                    cc.setDevice(nullptr);
                    win->enableControls(false);
                }
            }
        } 
    });

    win->setCallback(WM_KEYDOWN, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(!cc.deviceIsSet())
        {
            return;
        }

        if(wParam == VK_LEFT)
        {
            btnLeft->onClick(hwnd);
        } else if(wParam == VK_RIGHT)
        {
            btnRight->onClick(hwnd);
        } else if(wParam == VK_UP)
        {
            btnUp->onClick(hwnd);
        } else if(wParam == VK_DOWN)
        {
            btnDown->onClick(hwnd);
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

    camcon::finalize();
    return 0;
}

camcon::arg_parse::RTControlArgs processArgs(PWSTR pCmdLine)
{
    int argc;
    auto argv {CommandLineToArgvW(pCmdLine, &argc)};

    auto args { camcon::arg_parse::parseRTControlArgs(argc, argv)};

    if(!args.syntaxIsValid)
    {
        MessageBox(nullptr, L"Usage:\n\trt-control <device_index>", L"Syntax Error", MB_OK);
        args.indexWasProvided = false;
        return args;
    }

    return args;
}

std::shared_ptr<win32w::Window> createMainWindow()
{
    win32w::WindowBuilder wb{};
    wb.generateClassName();
    wb.createClass();
    auto style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    auto win_title = sys::str_tools::join({camcon::TITLE, camcon::VERSION_STR, L"Connected"});

    return wb.build(win_title, CW_USEDEFAULT, CW_USEDEFAULT, camcon::W_WIDTH, camcon::W_HEIGHT, style);
}