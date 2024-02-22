#include <win32_wrapper/control_factory.hpp>

namespace win32w
{

std::shared_ptr<Control> ControlFactory::createButton(std::shared_ptr<Window> parent, std::wstring text, int x, int y, int width, int height, DWORD style)
{
    auto hwnd = CreateWindow(
        WC_BUTTON,
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
    ctrl->type = ControlType::BUTTON;

    parent->addControl(ctrl);
    
    return ctrl;
}

std::shared_ptr<Control> ControlFactory::createComboBox(std::shared_ptr<Window> parent, int x, int y, int width, int height, DWORD style)
{
    auto hwnd = CreateWindow(
        WC_COMBOBOX,
        L"",
        style,
        x, y, width, height,
        parent->hwnd,
        nullptr,
        (HINSTANCE) GetWindowLongPtr(parent->hwnd, GWLP_HINSTANCE),
        nullptr
    );

    auto ctrl { std::make_shared<Control>() };

    ctrl->hwnd = hwnd;
    ctrl->type = ControlType::COMBO_BOX;

    parent->addControl(ctrl);
    
    return ctrl;
}

std::shared_ptr<Control> ControlFactory::createEdit(std::shared_ptr<Window> parent, int x, int y, int width, int height, DWORD style)
{
    auto hwnd = CreateWindow(
        WC_EDIT,
        L"",
        style,
        x, y, width, height,
        parent->hwnd,
        nullptr,
        (HINSTANCE) GetWindowLongPtr(parent->hwnd, GWLP_HINSTANCE),
        nullptr
    );

    auto ctrl { std::make_shared<Control>() };

    ctrl->hwnd = hwnd;
    ctrl->type = ControlType::EDIT;

    parent->addControl(ctrl);

    return ctrl;
}

std::shared_ptr<Control> ControlFactory::createTrackBar(std::shared_ptr<Window> parent, std::wstring text, int x, int y, int width, int height, int min, int max, DWORD style)
{
    auto hwnd = CreateWindow(
        TRACKBAR_CLASS,
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
    ctrl->type = ControlType::TRACKBAR;

    ctrl->setRange(min, max);

    parent->addControl(ctrl);

    return ctrl;
}

std::shared_ptr<Control> ControlFactory::createListBox(std::shared_ptr<Window> parent, int x, int y, int width, int height, DWORD style)
{
    auto hwnd = CreateWindow(
        WC_LISTBOX,
        L"",
        style,
        x, y, width, height,
        parent->hwnd,
        nullptr,
        (HINSTANCE) GetWindowLongPtr(parent->hwnd, GWLP_HINSTANCE),
        nullptr
    );

    auto ctrl { std::make_shared<Control>() };

    ctrl->hwnd = hwnd;
    ctrl->type = ControlType::LIST_BOX;

    parent->addControl(ctrl);
    
    return ctrl;
}

}