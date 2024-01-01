#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include <string>


namespace win32w
{

class FontBuilder
{
public:
    std::wstring family{L"Arial"};
    Gdiplus::FontStyle style{Gdiplus::FontStyleRegular};
    float size{12};

    Gdiplus::Font build();

};

}