#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>

#include<system/io.hpp>
#include<system/str_tools.hpp>

#include <strmif.h>

#include <iostream>
#include <string>

tagCameraControlProperty str_to_camConProp(std::string prop_name);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage:\n\tset_current_cam_config <device_index> <file>\n";
        return 1;
    }

    auto idx = std::atoi(argv[1]);
    std::string filename {argv[2]};

    camcon::initialize();

    camcon::VideoDeviceEnumerator vde{};

    auto devSrc {vde.getDevice(idx)};

    CComPtr<IAMCameraControl> devSrcControl { nullptr };
    devSrc->QueryInterface(IID_PPV_ARGS(&devSrcControl));

    auto lines = system_u::read_file_lines(filename);
    for(auto l : lines)
    {
        auto tokens = system_u::str_tools::split(l, ',');
        devSrcControl->Set(str_to_camConProp(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
    }

    camcon::finalize();

    return 0;
}

void print_property(CComPtr<IAMCameraControl> cc, std::string name, tagCameraControlProperty prop)
{
    long lvalue{-1};
    long flags{-1};
    cc->Get(prop, &lvalue, &flags);
    std::cout << name << ',' << lvalue << ',' << flags << '\n';
}

tagCameraControlProperty str_to_camConProp(std::string prop_name)
{
    if(prop_name == "pan")
        return CameraControl_Pan;
    if(prop_name == "tilt")
        return CameraControl_Tilt;
    if(prop_name == "roll")
        return CameraControl_Roll;
    if(prop_name == "zoom")
        return CameraControl_Zoom;
    if(prop_name == "exposure")
        return CameraControl_Exposure;
    if(prop_name == "iris")
        return CameraControl_Iris;
    if(prop_name == "focus")
        return CameraControl_Focus;

    return CameraControl_Pan;
}