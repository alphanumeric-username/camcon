#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include <system/str_tools.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    mfw::initialize();

    if(argc != 2)
    {
        std::cout << "Usage:\n\tget_current_cam_config <device_index>\n";
        return 1;
    }

    std::string idx_raw {argv[1]};
    if(!sys::str_tools::is_integer(idx_raw))
    {
        std::cout << "ERROR: index must be an integer\n";
        return 2;
    }

    auto idx = std::atoi(argv[1]);

    mfw::VideoDeviceEnumerator vde{};

    auto devSrc {vde.getDevice(idx)};

    mfw::CameraController cc{};

    cc.setDevice(devSrc);

    std::cout << "prop,min,max,delta,default,flags\n";
    std::cout << cc.getPropertyRange(CameraControl_Pan).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Tilt).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Roll).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Zoom).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Exposure).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Iris).toString() << '\n';
    std::cout << cc.getPropertyRange(CameraControl_Focus).toString() << '\n';

    mfw::finalize();

    return 0;
}