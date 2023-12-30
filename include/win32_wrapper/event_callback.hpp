#pragma once

#include <Windows.h>

namespace win32w
{

/// @brief Generic message callback for windows created with this library.
/// @param hwnd Window handle
/// @param uMsg Message code
/// @param wParam Event data. Depends on the message
/// @param lParam Event data. Depends on the message
/// @return 
/// @see WNDPROC
LRESULT eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

}