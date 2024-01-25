#include <win32_wrapper/control.hpp>

#include <windowsx.h>
#include <CommCtrl.h>

namespace win32w
{

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
    if(type == ControlType::TRACKBAR) {
        SendMessage(hwnd, TBM_SETRANGE, TRUE, MAKELONG(min, max));
    }
}

void Control::setTrackPosition(int pos)
{
    if(type == ControlType::TRACKBAR) {
        SendMessage(hwnd, TBM_SETPOS, TRUE, pos);
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

}