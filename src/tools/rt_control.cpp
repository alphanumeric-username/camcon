#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <win32_wrapper/init.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>

#include <system/str_tools.hpp>

#include <Dbt.h>
#include <ks.h>
#include <ksmedia.h>
#include <stringapiset.h>

#include <iostream>
#include <exception>
#include <string>

const int W_WIDTH = 800;
const int W_HEIGHT = 400;
const std::wstring VERSION_STR {L"v3.1"};
const std::wstring TITLE {L"CAMCON"};

HDEVNOTIFY registerForDeviceNotification(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    camcon::initialize();
    auto [gdiToken, gdiStartupInput] = win32w::init();

    int argc;
    auto argv {CommandLineToArgvW(pCmdLine, &argc)};

    if(argc != 1)
    {
        MessageBox(nullptr, L"Missing device index\n\nUsage:\n\trt-control <DEVICE_INDEX>", L"Error", MB_OK);
        return 1;
    }

    auto devIdx {std::stoi(argv[0])};

    camcon::VideoDeviceEnumerator vde{};

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
    camcon::CameraController cc{};
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

    win32w::ControlFactory cf{};
    auto btnLeft = cf.createButton(win, L"<", (int) center_x - btn_padding - btn_size, center_y, btn_size, btn_size);
    auto btnRight = cf.createButton(win, L">", (int) center_x + btn_padding + btn_size, center_y, btn_size, btn_size);
    auto btnUp = cf.createButton(win, L"^", (int) center_x, center_y  - btn_padding - btn_size, btn_size, btn_size);
    auto btnDown = cf.createButton(win, L"v", (int) center_x, center_y + btn_padding + btn_size, btn_size, btn_size);
    auto btnIn = cf.createButton(win, L"+", (int) center_x + btn_padding + btn_size, center_y  - btn_padding - btn_size, btn_size, btn_size);
    auto btnOut = cf.createButton(win, L"-", (int) center_x + btn_padding + btn_size, center_y  + btn_padding + btn_size, btn_size, btn_size);

    

    btnLeft->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Pan);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
        }
    };
    
    btnRight->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Pan);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
    };
    
    btnUp->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Tilt);
        // auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Exposure);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
    };
    
    btnDown->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Tilt);
        // auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Exposure);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
        }
    };
    
    btnIn->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Zoom);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue + 1);
        }
    };
    
    btnOut->onClick = [&](HWND hwnd) {
        auto prop = cc.getProperty(tagCameraControlProperty::CameraControl_Zoom);
        if(prop.valid)
        {
            cc.setProperty(prop.prop, prop.lvalue - 1);
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

            // std::wstring rightSymbLink {system_u::str_tools::stows(dbdi->dbcc_name)};
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

    camcon::finalize();
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