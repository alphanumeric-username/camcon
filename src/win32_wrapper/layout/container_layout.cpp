#include <win32_wrapper/layout/container_layout.hpp>

namespace win32w
{

float ContainerLayout::itemX() { return itemX_; }

float ContainerLayout::itemY() { return itemY_; }

void ContainerLayout::updateLayout()
{
    float centerX = x_ + width_/2;
    float centerY = y_ + height_/2;

    itemX_ = centerX - itemWidth/2;
    itemY_ = centerY - itemHeight/2;
}

}