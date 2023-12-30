#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <system/str_tools.hpp>

#include <strmif.h>

#include <iostream>
#include <string>

void print_property(CComPtr<IAMCameraControl> cc, std::string name, tagCameraControlProperty prop);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage:\n\tget_current_cam_config <device_index>\n";
        return 1;
    }

    std::string idx_raw {argv[1]};
    if(!system_u::str_tools::is_integer(idx_raw))
    {
        std::cout << "ERROR: index must be an integer\n";
        return 2;
    }

    auto idx = std::atoi(argv[1]);

    camcon::initialize();

    camcon::VideoDeviceEnumerator vde{};
    camcon::CameraController camController{};
    
    if(idx < 0 || idx >= vde.count())
    {
        std::cout << "ERROR: index out of bounds\n";
        return 3;
    }

    auto devSrc {vde.getDevice(idx)};

    camController.setDevice(devSrc);

    std::cout << camController.getProperty(CameraControl_Pan).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Tilt).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Roll).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Zoom).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Exposure).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Iris).toString() << '\n';
    std::cout << camController.getProperty(CameraControl_Focus).toString() << '\n';

    camcon::finalize();

    return 0;
}