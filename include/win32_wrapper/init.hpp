#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include <tuple>

namespace win32w
{

/// @brief Initializes GDI+
/// @return 
std::tuple<ULONG_PTR, Gdiplus::GdiplusStartupInput> init();

/// @brief Terminates GDI+
/// @param gdiplusToken 
void finalize(ULONG_PTR gdiplusToken);

}