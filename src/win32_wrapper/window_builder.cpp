#include <win32_wrapper/window_builder.hpp>
#include <win32_wrapper/window_registry.hpp>
#include <win32_wrapper/event_callback.hpp>

// #include <win32_wrapper/control_map.hpp>
// #include <win32_wrapper/callback_map.hpp>
// #include <win32_wrapper/gdi_text_map.hpp>

#include <Windows.h>

#include <string>
#include <map>
#include <vector>
#include <sstream>

namespace win32w
{

LRESULT mockCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WindowBuilder::WindowBuilder() {}

int next_class_number = 0;
void WindowBuilder::generateClassName()
{
    className_ = L"W32_WND_CLASS_N" + std::to_wstring(next_class_number++);
}

void WindowBuilder::createClass()
{
    WNDCLASS wndClass {};

    wndClass.lpszClassName = className_.c_str();
    wndClass.lpfnWndProc = eventCallback;
    // wndClass.lpfnWndProc = mockCallback;
    wndClass.hInstance = GetModuleHandle(nullptr);
    // wndClass.hbrBackground = CreateSolidBrush(COLOR_WINDOW + 1);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    auto r = RegisterClass(&wndClass);
}


std::shared_ptr<Window> WindowBuilder::build(std::wstring title, int x, int y, int width, int height, DWORD style)
{
    HWND hwnd = CreateWindowEx(
        0,
        className_.c_str(),
        title.c_str(),
        style,
        x,
        y,
        width,
        height,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    auto window { std::make_shared<Window>() };
    window->hwnd = hwnd;

    window_registry::registerWindow(window);

    return window;
}

void WindowBuilder::setClassName(std::wstring className)
{
    className_ = className;
}

}