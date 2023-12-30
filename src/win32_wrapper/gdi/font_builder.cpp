#include <win32_wrapper/gdi/font_builder.hpp>

#pragma comment(lib, "Gdiplus.lib")

namespace win32w
{

Gdiplus::Font FontBuilder::build()
{
    Gdiplus::FontFamily ff{family.c_str()};
    return Gdiplus::Font{&ff, size, style, Gdiplus::UnitPixel};
}

}