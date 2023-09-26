#include <mf_wrapper/init.hpp>
#include <mf_wrapper/vde.hpp>

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

    auto idx = std::atoi(argv[1]);

    camcon::initialize();

    camcon::VideoDeviceEnumerator vde{};

    auto devSrc {vde.getDevice(idx)};

    CComPtr<IAMCameraControl> devSrcControl { nullptr };
    devSrc->QueryInterface(IID_PPV_ARGS(&devSrcControl));//QueryInterface<IAMCameraControl>(devSrcControl);

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