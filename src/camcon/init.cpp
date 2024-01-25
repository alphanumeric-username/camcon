#include <camcon/init.hpp>
#include <mf_wrapper/init.hpp>
#include <win32_wrapper/init.hpp>

#include <gdiplus.h>

#include <tuple>

namespace camcon
{

ULONG_PTR gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;

void init()
{
    mfw::initialize();
    std::tie(gdiplusToken, gdiplusStartupInput) = win32w::init();
}

void finalize()
{
    win32w::finalize(gdiplusToken);
    mfw::finalize();
}

}