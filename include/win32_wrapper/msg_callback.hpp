#pragma once

#include <Windows.h>

#include <functional>

namespace win32w
{

/// @brief Window a message callback. Can be added to a Window to process the specified message.
typedef std::function<void(HWND, UINT, WPARAM, LPARAM)> MsgCallback;

}