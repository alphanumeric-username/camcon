#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>
#include <mf_wrapper/camera_controller.hpp>

#include<system/io.hpp>
#include<system/str_tools.hpp>

#include <strmif.h>

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage:\n\tset_current_cam_config <device_index> <file>\n";
        return 1;
    }

    std::string idx_raw {argv[1]};
    if(!sys::str_tools::is_integer(idx_raw))
    {
        std::cout << "ERROR: index must be an integer\n";
        return 2;
    }

    auto idx = std::atoi(argv[1]);
    std::string filename {argv[2]};

    mfw::initialize();

    mfw::VideoDeviceEnumerator vde{};

    if(idx < 0 || idx >= vde.count())
    {
        std::cout << "ERROR: index out of bounds\n";
        return 3;
    }

    auto devSrc {vde.getDevice(idx)};

    mfw::CameraController camController{};
    camController.setDevice(devSrc);

    auto lines = sys::read_file_lines(filename);
    for(auto l : lines)
    {
        try {
            auto propVal = mfw::parsePropConfigString(l);
            if(propVal.valid)
            {
                camController.setProperty(propVal.prop, propVal.lvalue);
            }
        } catch(std::exception e) {}
    }

    mfw::finalize();

    return 0;
}