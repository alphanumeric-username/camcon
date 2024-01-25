#pragma once

#include <win32_wrapper/layout/layout_element.hpp>

#include <win32_wrapper/control.hpp>

namespace win32w
{

class ControlContainer : LayoutElement
{
private:
    std::shared_ptr<Control> ctrl_{ nullptr };

public:

    void setControl(std::shared_ptr<Control> ctrl);

    virtual void updateLayout() override;

};

}