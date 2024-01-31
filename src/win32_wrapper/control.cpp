#include <win32_wrapper/control.hpp>

#include <windowsx.h>
#include <CommCtrl.h>

namespace win32w
{

void Control::clearItems()
{
    if(type == ControlType::COMBO_BOX) {
        ComboBox_ResetContent(this->hwnd);
    }
}

void Control::addItem(std::wstring item)
{
    if(type == ControlType::COMBO_BOX) {
        ComboBox_AddString(this->hwnd, item.c_str());
    }
}

void Control::addItem(std::vector<std::wstring> items)
{
    for(auto& i : items)
    {
        addItem(i);
    }
}

void Control::setRange(UINT min, UINT max)
{
    if(type == ControlType::TRACKBAR) 
    {
        SendMessage(hwnd, TBM_SETRANGE, TRUE, MAKELONG(min, max));
    }
}

void Control::setTrackPosition(int pos)
{
    if(type == ControlType::TRACKBAR) 
    {
        SendMessage(hwnd, TBM_SETPOS, TRUE, pos);
    }
}

int win32w::Control::getTrackPosition()
{
    if(type == ControlType::TRACKBAR) 
    {
        return SendMessage(hwnd, TBM_GETPOS, 0, 0);
    } else 
    {
        return 0;
    }
}


std::wstring Control::getText()
{
    std::wstring text {L""};

    auto len = GetWindowTextLength(hwnd);
    wchar_t* tbuffer = new wchar_t[len + 1] {};

    GetWindowText(hwnd, tbuffer, len + 1);
    text += tbuffer;

    delete tbuffer;

    return text;
}

void Control::setText(std::wstring text)
{
    SetWindowText(hwnd, text.c_str());
}

void win32w::Control::setRectangle(int x, int y, int width, int height)
{
    if(type == ControlType::COMBO_BOX)
    {
        SetWindowPos(hwnd, HWND_TOP, x, y, width, 160, SWP_ASYNCWINDOWPOS);
        ComboBox_SetItemHeight(hwnd, -1, height);
    } else {
        SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_ASYNCWINDOWPOS);
    }
}


MsgCallback Control::getCallback(UINT uMsg)
{
    if (callbackMap_.count(uMsg) > 0)
    {
        return callbackMap_.at(uMsg);
    }

    return [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {};
}


void Control::setCallback(UINT uMsg, MsgCallback callback)
{
    callbackMap_.insert_or_assign(uMsg, callback);
}

}