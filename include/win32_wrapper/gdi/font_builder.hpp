#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include <string>


namespace win32w
{

class FontBuilder
{
public:
    std::wstring family{};
    Gdiplus::FontStyle style{Gdiplus::FontStyleRegular};
    float size{12};

    Gdiplus::Font build();

};

}