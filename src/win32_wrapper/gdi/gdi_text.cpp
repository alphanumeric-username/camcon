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

    Gdiplus::StringFormat sf{};

    if(alignment & TOP) 
    {
        sf.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentNear);
    } else if (alignment & MIDDLE) 
    {
        sf.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
    }

    if (alignment & CENTER)
    {
        sf.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
    } else if(alignment & RIGHT)
    {
        sf.SetAlignment(Gdiplus::StringAlignment::StringAlignmentFar);
    }

    auto font = fontBuilder.build();

    if(useRectangle) {
        Gdiplus::RectF rect{ x, y, width, height };

        g.DrawString(contents.c_str(), -1, &font, rect, &sf, &brush);
    } else {
        g.DrawString(contents.c_str(), -1, &font, p, &sf, &brush);
    }
}

}