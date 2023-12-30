#pragma once

#include <win32_wrapper/gdi/font_builder.hpp>

#include <Windows.h>
#include <gdiplus.h>
// #include <gdiplusheaders.h>
// #include <gdiplusenums.h>

#include <array>
#include <string>


namespace win32w
{

class GDIText
{
public:
    FontBuilder fontBuilder{};
    std::wstring contents {L""};
    float x {0};
    float y {0};
    std::array<int, 4> color {0,0,0,255};
    
    void onPaint(HDC hdc);
};

}