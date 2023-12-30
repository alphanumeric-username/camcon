#pragma once

#include <Windows.h>

#include <functional>

namespace win32w
{

/// @brief Represents a Windows Controls object
class Control
{
public:
    /// @brief Window handle to the object
    HWND hwnd;

    /// @brief Called when the Control is clicked
    std::function<void(HWND)> onClick{[](HWND hwnd) {}};
    
};

}