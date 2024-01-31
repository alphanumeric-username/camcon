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

enum GDITextAlignment {
    BOTTOM = 0b00000000,
    MIDDLE = 0b00000001,
    TOP    = 0b00000010,
    LEFT   = 0b00000000,
    CENTER = 0b00010000,
    RIGHT  = 0b00100000,
};

// enum GDITextAlignment {
//     TOP_LEFT, TOP_CENTER, TOP_RIGHT,
//     MIDDLE_LEFT, MIDDLE_CENTER, MIDDLE_RIGHT,
//     BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
// };

class GDIText
{
public:
    FontBuilder fontBuilder{};
    std::wstring contents {L""};
    int alignment {BOTTOM | LEFT};
    float x {0};
    float y {0};
    float width{0};
    float height{0};

    bool useRectangle {false};
    
    std::array<int, 4> color {0,0,0,255};
    
    void onPaint(HDC hdc);
};

}