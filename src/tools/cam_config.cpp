#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>

#include<system/io.hpp>
#include<system/str_tools.hpp>

#include <strmif.h>

#include <iostream>
#include <string>
#include <exception>
#include <cstdio>

int parse_args(int argc, char* argv[]);

void cmd_get(int idx);
void cmd_set(int idx, std::string filename);

void print_property(CComPtr<IAMCameraControl> cc, std::string name, tagCameraControlProperty prop);
tagCameraControlProperty str_to_camConProp(std::string prop_name);

int main(int argc, char* argv[])
{
    camcon::initialize();

    parse_args(argc, argv);

    camcon::finalize();

    return 0;
}

void cmd_get(int idx)
{
    camcon::VideoDeviceEnumerator vde{};

    auto devSrc {vde.getDevice(idx)};

    CComPtr<IAMCameraControl> devSrcControl { nullptr };
    devSrc->QueryInterface(IID_PPV_ARGS(&devSrcControl));

    long lvalue{-1};
    long flags{-1};

    devSrcControl->Get(CameraControl_Pan, &lvalue, &flags);

    print_property(devSrcControl, "pan", CameraControl_Pan);
    print_property(devSrcControl, "tilt", CameraControl_Tilt);
    print_property(devSrcControl, "roll", CameraControl_Roll);
    print_property(devSrcControl, "zoom", CameraControl_Zoom);
    print_property(devSrcControl, "exposure", CameraControl_Exposure);
    print_property(devSrcControl, "iris", CameraControl_Iris);
    print_property(devSrcControl, "focus", CameraControl_Focus);
}

void cmd_set(int idx, std::string filename)
{
    camcon::VideoDeviceEnumerator vde{};

    auto devSrc {vde.getDevice(idx)};

    CComPtr<IAMCameraControl> devSrcControl { nullptr };
    devSrc->QueryInterface(IID_PPV_ARGS(&devSrcControl));

    auto lines = system_u::read_file_lines(filename);
    for(auto l : lines)
    {
        try
        {
            auto tokens = system_u::str_tools::split(l, ',');
            devSrcControl->Set(str_to_camConProp(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
        } catch(std::exception e) {}
    }
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