#pragma once

#include <Windows.h>

#include <string>
#include <vector>
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
    
    /// @brief Called when an item is selected
    std::function<void(HWND)> onSelect{[](HWND hwnd) {}};
    
    void addItem(std::wstring item);
};

}