#include <mf_wrapper/camera_controller.hpp>
#include <system/str_tools.hpp>

#include <sstream>

namespace camcon
{

void CameraController::setDevice(CComPtr<IMFMediaSource> devSrc)
{
    control_ = nullptr;
    devSrc->QueryInterface(IID_PPV_ARGS(&control_));
}

bool CameraController::deviceIsSet()
{
    return control_ != nullptr;
}

CameraControlPropertyValue CameraController::getProperty(tagCameraControlProperty prop)
{
    CameraControlPropertyValue propVal{};
    propVal.prop = prop;

    if(!deviceIsSet())
    {
        propVal.valid = false;
        return propVal;
    }

    control_->Get(prop, &(propVal.lvalue), &(propVal.flags));

    return propVal;
}

void CameraController::setProperty(tagCameraControlProperty prop, long value)
{
    if(!deviceIsSet())
    {
        return;
    }
    auto hr = control_->Set(prop, value, tagCameraControlFlags::CameraControl_Flags_Manual);
}

std::string propToStr(tagCameraControlProperty prop)
{
    if(prop == CameraControl_Pan)
        return "pan";
    if(prop == CameraControl_Tilt)
        return "tilt";
    if(prop == CameraControl_Roll)
        return "roll";
    if(prop == CameraControl_Zoom)
        return "zoom";
    if(prop == CameraControl_Exposure)
        return "exposure";
    if(prop == CameraControl_Iris)
        return "iris";
    if(prop == CameraControl_Focus)
        return "focus";

    return "";
}

tagCameraControlProperty strToProp(std::string prop)
{
    if(prop == "pan")
        return CameraControl_Pan;
    if(prop == "tilt")
        return CameraControl_Tilt;
    if(prop == "roll")
        return CameraControl_Roll;
    if(prop == "zoom")
        return CameraControl_Zoom;
    if(prop == "exposure")
        return CameraControl_Exposure;
    if(prop == "iris")
        return CameraControl_Iris;
    if(prop == "focus")
        return CameraControl_Focus;

    return CameraControl_Pan;
}

std::string CameraControlPropertyValue::toString()
{
    std::stringstream ss{};
    ss << propToStr(prop) << "," << lvalue << "," << flags;
    return ss.str();
}

CameraControlPropertyValue parsePropConfigString(std::string str)
{
    CameraControlPropertyValue propVal{};
    
    try {
        auto tokens = system_u::str_tools::split(str, ',');
        propVal.prop = strToProp(tokens.at(0));
        propVal.lvalue = std::stoi(tokens.at(1));
        propVal.flags = std::stoi(tokens.at(2));
    } catch(std::exception e) {
        propVal.valid = false;
    }

    return propVal;
}

}