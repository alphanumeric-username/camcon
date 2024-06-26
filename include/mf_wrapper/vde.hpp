#pragma once

#include<mfidl.h>
#include<atlbase.h>
#include<atlcom.h>

#include<vector>
#include<string>

namespace mfw
{

class VideoDeviceEnumerator
{
public:
    VideoDeviceEnumerator();
    
    void enumerateDevices();
    UINT32 count() const;
    CComPtr<IMFMediaSource> getDevice(int idx);
    CComPtr<IMFMediaSource> getDeviceFromSymbolicLink(std::wstring symbolic_link);
    std::wstring getDeviceName(int idx);
    std::vector<CComPtr<IMFMediaType>> getMediaTypes(int idx);
    std::wstring getSymbolicLink(int idx);

    std::vector<std::wstring> getDeviceNames();
    
    
    void checkIndexInBounds(int idx);
    void release();

private:
    IMFActivate** videoCapDevs_ { nullptr };
    UINT32 dev_count_{0};
};

}