#pragma once

#include<mfidl.h>
#include<atlbase.h>
#include<atlcom.h>

#include <strmif.h>

#include <string>

namespace mfw
{

struct CameraControlPropertyValue
{
    tagCameraControlProperty prop{};
    long lvalue{};
    long flags{};
    bool valid{true};

    std::string toString();
};

struct CameraControlPropertyRange
{
    tagCameraControlProperty prop{};
    long pMin {0};
    long pMax {0};
    long pSteppingDelta {0};
    long pDefault {0};
    long pCapsFlags {0};
    bool valid{true};
    
    std::string toString();
};

class CameraController
{
private:
    CComPtr<IAMCameraControl> control_{nullptr};

public:
    void setDevice(CComPtr<IMFMediaSource> devSrc);
    bool deviceIsSet();

    CameraControlPropertyValue getProperty(tagCameraControlProperty prop);
    CameraControlPropertyRange getPropertyRange(tagCameraControlProperty prop);
    void setProperty(tagCameraControlProperty prop, long value);
};

std::string propToStr(tagCameraControlProperty prop);
tagCameraControlProperty strToProp(std::string prop);

CameraControlPropertyValue parsePropConfigString(std::string str);

}
