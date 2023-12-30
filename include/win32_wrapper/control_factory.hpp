#pragma once

#include <win32_wrapper/window.hpp>
#include <win32_wrapper/control.hpp>

#include <Windows.h>

#include <string>
#include <memory>

namespace win32w
{

/// @brief Manages the creation of Windows Controls objects
class ControlFactory
{
public:
    /// @brief Creates a Windows Controls' Button object for a Window.
    /// @param parent Pointer to the Window object to be created
    /// @param text Button text
    /// @param x x-position in pixels
    /// @param y y-position in pixels
    /// @param width width in pixels
    /// @param height height in pixels
    /// @param style Button style flags
    /// @return A pointer to the created Button
    std::shared_ptr<Control> createButton(std::shared_ptr<Window> parent, std::wstring text, int x, int y, int width, int height, 
        DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
};

}