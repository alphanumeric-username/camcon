#include <win32_wrapper/gdi/gdi_text.hpp>

// #include <gdiplusgraphics.h>
// #include <gdiplusbrush.h>
#pragma comment(lib, "Gdiplus.lib")

namespace win32w
{

void GDIText::onPaint(HDC hdc)
{
    Gdiplus::Graphics g{hdc};
    Gdiplus::SolidBrush brush 
    {
        Gdiplus::Color(color[3], color[0], color[1], color[2])
    };
    Gdiplus::PointF p{x, y};

    auto font = fontBuilder.build();
    g.DrawString(contents.c_str(), -1, &font, p, &brush);
}

}