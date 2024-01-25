#include <mf_wrapper/vde.hpp>

#include <mf_wrapper/attribute.hpp>

#include <exception>

namespace mfw
{

VideoDeviceEnumerator::VideoDeviceEnumerator()
{
    enumerateDevices();
}

void VideoDeviceEnumerator::enumerateDevices()
{
    if(videoCapDevs_ != nullptr)
    {
        release();
    }

    videoCapDevs_ = nullptr;

    Attribute attr{};

    attr.setAttributeGUID(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, 
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
    );

    auto hr = MFEnumDeviceSources(attr.pAttributes(), &videoCapDevs_, &dev_count_);
    
    if(FAILED(hr))
    {
        throw std::exception{
            "[VideoDeviceEnumerator]: Couldn't enumerate devices"
        };
    }
}


UINT32 VideoDeviceEnumerator::count() const
{
    return dev_count_;
}

CComPtr<IMFMediaSource> VideoDeviceEnumerator::getDevice(int idx)
{
    checkIndexInBounds(idx);

    CComPtr<IMFMediaSource> pMediaSrc {nullptr};

    videoCapDevs_[idx]->ActivateObject(IID_PPV_ARGS(&pMediaSrc));
    
    return pMediaSrc;
}


CComPtr<IMFMediaSource> VideoDeviceEnumerator::getDeviceFromSymbolicLink(std::wstring symbolic_link)
{
    CComPtr<IMFActivate> pActivate { nullptr };
    CComPtr<IMFMediaSource> pMediaSrc { nullptr };
    
    Attribute attr{};

    attr.setAttributeGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    attr.setAttributeWSTR(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, symbolic_link);

    auto hr = MFCreateDeviceSourceActivate(attr.pAttributes(), &pActivate);
    if (FAILED(hr))
    {
        return nullptr;
    }

    hr = pActivate->ActivateObject(IID_PPV_ARGS(&pMediaSrc));

    return FAILED(hr) ? nullptr : pMediaSrc;
}


std::wstring VideoDeviceEnumerator::getDeviceName(int idx)
{
    checkIndexInBounds(idx);

    LPWSTR dev_name{nullptr};
    UINT32 length{0};
    videoCapDevs_[idx]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &dev_name, &length);
    return std::wstring { dev_name };
}


std::wstring VideoDeviceEnumerator::getSymbolicLink(int idx)
{
    checkIndexInBounds(idx);

    LPWSTR symbolic_link{nullptr};
    UINT32 length{0};
    videoCapDevs_[idx]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &symbolic_link, &length);
    return std::wstring { symbolic_link };
}


std::vector<CComPtr<IMFMediaType>> VideoDeviceEnumerator::getMediaTypes(int idx)
{
    auto pDevSrc {getDevice(idx)};

    CComPtr<IMFPresentationDescriptor> pDescriptor{nullptr};
    pDevSrc->CreatePresentationDescriptor(&pDescriptor);

    // pDescriptor->GetStreamDescriptorByIndex()

    return std::vector<CComPtr<IMFMediaType>>{};
}

void VideoDeviceEnumerator::checkIndexInBounds(int idx)
{
    if(idx < 0 || idx >= dev_count_)
    {
        throw std::exception {
            "[VideoDeviceEnumerator]: Index out of bounds"
        };
    }
}


void VideoDeviceEnumerator::release()
{
    for(int i = 0; i < dev_count_; i++)
    {
        videoCapDevs_[i]->Release();
    }

    CoTaskMemFree(videoCapDevs_);
}

}