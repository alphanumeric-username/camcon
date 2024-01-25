#pragma once

#include <mf_wrapper/camera_controller.hpp>

#include <Windows.h>

#include <filesystem>


namespace camcon
{

namespace fs = std::filesystem;

HDEVNOTIFY registerForDeviceNotification(HWND hwnd);
fs::path getExePath();
void varyCamProp(mfw::CameraController& cc, tagCameraControlProperty ccp, int delta);


}