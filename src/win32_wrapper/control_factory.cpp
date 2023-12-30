#include <win32_wrapper/control_factory.hpp>

namespace win32w
{

std::shared_ptr<Control> ControlFactory::createButton(std::shared_ptr<Window> parent, std::wstring text, int x, int y, int width, int height, 
    DWORD style)
{
    auto hwnd = CreateWindow(
        L"BUTTON",
        text.c_str(),
        style,
        x, y, width, height,
        parent->hwnd,
        nullptr,
        (HINSTANCE) GetWindowLongPtr(parent->hwnd, GWLP_HINSTANCE),
        nullptr
    );

    auto ctrl { std::make_shared<Control>() };

    ctrl->hwnd = hwnd;
    parent->addControl(ctrl);
    
    return ctrl;
}

}