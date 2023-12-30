#include <win32_wrapper/window.hpp>

namespace win32w
{


void Window::addControl(std::shared_ptr<Control> ctrl)
{
    controlList_.emplace_back(ctrl);
}

void Window::addGDIText(std::shared_ptr<GDIText> gdiText)
{
    gdiTextList_.emplace_back(gdiText);
}

void Window::setCallback(UINT uMsg, MsgCallback callback)
{
    callbackMap_.insert_or_assign(uMsg, callback);
}

std::vector<std::shared_ptr<Control>> Window::getControlList()
{
    return controlList_;
}

std::vector<std::shared_ptr<GDIText>> Window::getGdiTextList()
{
    return gdiTextList_;
}

MsgCallback Window::getCallback(UINT uMsg)
{
    if (callbackMap_.count(uMsg) > 0)
    {
        return callbackMap_.at(uMsg);
    }

    return [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {};
}

}