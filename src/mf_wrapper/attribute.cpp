#include <mf_wrapper/attribute.hpp>

#include <mfapi.h>


namespace camcon
{

Attribute::Attribute()
{
    auto hr = MFCreateAttributes(&(pAttributes_), 10);
    if(FAILED(hr))
    {
        throw std::exception{
            "[Attribute]: Couldn't create IMFAttributes"
        };
    }

}

std::tuple<HRESULT, std::wstring> GetAttributeWSTR(IMFAttributes* pAttributes, GUID key)
{
    UINT32 str_length{0};
    wchar_t* string_raw = nullptr;

    auto hr = pAttributes->GetAllocatedString(key, &string_raw, &str_length);
    if(FAILED(hr))
    {
        return {hr, L""};
    }

    std::wstring str {string_raw};
    
    CoTaskMemFree(string_raw);

    return {hr, str};
}

HRESULT Attribute::setAttributeWSTR(GUID key, std::wstring value)
{
    return pAttributes_->SetString(key, value.c_str());
}

std::tuple<HRESULT, GUID> Attribute::getAttributeGUID(GUID key)
{
    GUID value{};
    auto hr {pAttributes_->GetGUID(key, &value)};
    return {hr, value};
}

HRESULT Attribute::setAttributeGUID(GUID key, GUID value)
{
    return pAttributes_->SetGUID(key, value);
}


const CComPtr<IMFAttributes> Attribute::pAttributes()
{
    return pAttributes_;
}

}