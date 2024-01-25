#include <camcon/utils.hpp>

#include <Dbt.h>
#include <ks.h>
#include <ksmedia.h>

namespace camcon
{

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

void varyCamProp(mfw::CameraController& cc, tagCameraControlProperty ccp, int delta)
{
    
}

}