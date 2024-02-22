#include <camcon/rt_control/select_device_window.hpp>

#include <camcon/constants.hpp>
#include <camcon/utils.hpp>
#include <mf_wrapper/vde.hpp>
#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/control_factory.hpp>

#include <Dbt.h>

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

const auto SELECT_DEV_WSTR { L"<Select a device>" };

int promptDeviceIndexWindow()
{
    mfw::VideoDeviceEnumerator vde{};
    int devIdx{-1};

    auto selWin = createWindow();
    

    win32w::ControlFactory cf{};
    auto cbSelDev = cf.createComboBox(selWin, 20, 20, camcon::W_WIDTH/2 - 40, 160);
    auto btnSelDev = cf.createButton(selWin, L"Select", camcon::W_WIDTH/2 - 160 - 20, 60, 160, 32);
    
    cbSelDev->addItem(vde.getDeviceNames());
    cbSelDev->setText(SELECT_DEV_WSTR);
    
    btnSelDev->setEnabled(false);

    cbSelDev->setCallback(CBN_SELCHANGE, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        auto currIdx = cbSelDev->getCurrentItemIndex();
        if(currIdx == CB_ERR)
        {
            return;
        }

        btnSelDev->setEnabled(true);
    });

    btnSelDev->setCallback(BN_CLICKED, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // auto currIdx = ComboBox_GetCurSel(cbSelDev->hwnd);
        auto currIdx = cbSelDev->getCurrentItemIndex();
        if(currIdx == CB_ERR)
        {
            return;
        }

        devIdx = currIdx;

        DestroyWindow(selWin->hwnd);
    });

    auto windHdevNotify = camcon::registerForDeviceNotification(selWin->hwnd);
    selWin->setCallback(WM_DEVICECHANGE, [&](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVICEARRIVAL)
        {
            cbSelDev->clearItems();

            vde.enumerateDevices();

            cbSelDev->addItem(vde.getDeviceNames());
            cbSelDev->setText(SELECT_DEV_WSTR);
            btnSelDev->setEnabled(false);
        } 
    });

    ShowWindow(selWin->hwnd, SW_NORMAL);

    MSG msg{};
    while(GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    windHdevNotify && UnregisterDeviceNotification(windHdevNotify);

    vde.release();

    return devIdx;
}

}