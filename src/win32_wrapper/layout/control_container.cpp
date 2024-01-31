#include <win32_wrapper/layout/control_container.hpp>

namespace win32w
{

ControlContainer::ControlContainer() {}

ControlContainer::ControlContainer(std::shared_ptr<Control> ctrl) {
    setControl(ctrl);
}


void ControlContainer::setControl(std::shared_ptr<Control> ctrl)
{
    ctrl_ = ctrl;
}

void ControlContainer::updateLayout()
{
    int x { static_cast<int>(x_) };
    int y { static_cast<int>(y_) };
    int w { static_cast<int>(width_) };
    int h { static_cast<int>(height_) };

    ctrl_->setRectangle(x, y, w, h);
    // SetWindowPos(ctrl_->hwnd, HWND_TOP, x, y, w, h, SWP_ASYNCWINDOWPOS);
}

}