#include <win32_wrapper/init.hpp>

#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")

namespace win32w
{

std::tuple<ULONG_PTR, Gdiplus::GdiplusStartupInput> init()
{
    ULONG_PTR gdiplusToken{};
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    return {gdiplusToken, gdiplusStartupInput};
}

void finalize(ULONG_PTR gdiplusToken)
{
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

}