#include <mf_wrapper/vde.hpp>

#include <mf_wrapper/attribute.hpp>

#include <exception>

namespace camcon
{

VideoDeviceEnumerator::VideoDeviceEnumerator()
{
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


std::wstring VideoDeviceEnumerator::getDeviceName(int idx)
{
    checkIndexInBounds(idx);

    LPWSTR dev_name{nullptr};
    UINT32 length{0};
    videoCapDevs_[idx]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &dev_name, &length);
    return std::wstring { dev_name };
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