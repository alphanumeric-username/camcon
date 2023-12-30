#include <win32_wrapper/window_registry.hpp>

namespace win32w::window_registry
{

std::map<HWND, std::shared_ptr<Window>> windowMap_{};

void registerWindow(std::shared_ptr<Window> window)
{
    HWND key = window->hwnd;
    windowMap_.insert_or_assign(key, window);
}

std::shared_ptr<Window> getWindow(HWND hwnd)
{
    if(windowMap_.count(hwnd) > 0) {
        return windowMap_.at(hwnd);
    }

    return nullptr;
}

}