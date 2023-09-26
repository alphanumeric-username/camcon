#pragma once

#include <mfobjects.h>
#include <atlbase.h>

#include <string>
#include <tuple>

namespace camcon
{

class Attribute
{
private:
    CComPtr<IMFAttributes> pAttributes_{};

public:
    Attribute();

    HRESULT setAttributeWSTR(GUID key, std::wstring value);
    HRESULT setAttributeGUID(GUID key, GUID value);

    std::tuple<HRESULT, std::wstring> getAttributeWSTR(GUID key);
    std::tuple<HRESULT, GUID> getAttributeGUID(GUID key);
    const CComPtr<IMFAttributes> pAttributes();
};

}
