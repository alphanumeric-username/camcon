#include <camcon/rt_control/select_device_window.hpp>

#include <camcon/constants.hpp>
#include <mf_wrapper/vde.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>

#include <Windows.h>
#include <windowsx.h>

namespace camcon
{

std::shared_ptr<win32w::Window> createWindow()
{
    win32w::WindowBuilder wb{};
    wb.generateClassName();
    wb.createClass();
    auto style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    auto win_title = L"Select a device:";

    return wb.build(win_title, CW_USEDEFAULT, CW_USEDEFAULT, camcon::W_WIDTH/2, camcon::W_HEIGHT/2, style);
}


int promptDeviceIndexWindow()
{
    mfw::VideoDeviceEnumerator vde{};
    int devIdx{-1};

    auto selWin = createWindow();

    win32w::ControlFactory cf{};
    auto cbSelDev = cf.createComboBox(selWin, 20, 20, camcon::W_WIDTH/2 - 40, 160);
    auto btnSelDev = cf.createButton(selWin, L"Select", camcon::W_WIDTH/2 - 160 - 20, 60, 160, 32);
    

    for(int i = 0; i < vde.count(); i++)
    {
        cbSelDev->addItem(vde.getDeviceName(i));
    }


    btnSelDev->onClick = [&](HWND hwnd)
    {
        auto currIdx = ComboBox_GetCurSel(cbSelDev->hwnd);
        if(currIdx == CB_ERR)
        {
            return;
        }

        devIdx = currIdx;

        DestroyWindow(selWin->hwnd);
    };
    

    ShowWindow(selWin->hwnd, SW_NORMAL);

    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    vde.release();

    return devIdx;
}

}