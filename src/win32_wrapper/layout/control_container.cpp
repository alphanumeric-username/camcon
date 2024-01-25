#include <win32_wrapper/layout/control_container.hpp>

namespace win32w
{

void ControlContainer::setControl(std::shared_ptr<Control> ctrl)
{
    ctrl_ = ctrl;
}

void ControlContainer::updateLayout()
{
    
}

}