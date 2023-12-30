#pragma once

#include <win32_wrapper/window.hpp>

#include <map>
#include <memory>

namespace win32w::window_registry
{

/// @brief Registers a Window to this wrapper system. This is usualy called by a WindowBuilder in the `build()` method.
/// @param window A pointer to the newly created window
void registerWindow(std::shared_ptr<Window> window);

/// @brief Finds a pointer to the registered Window with the provided handle.
/// @param hwnd Window handle
/// @return If the window was registered, returns a pointer to the Window. Otherwise it returns `nullptr`.
std::shared_ptr<Window> getWindow(HWND hwnd);

}