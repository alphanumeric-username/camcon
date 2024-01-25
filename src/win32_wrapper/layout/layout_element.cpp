#include <win32_wrapper/layout/layout_element.hpp>

namespace win32w
{

void LayoutElement::setPosition(float x, float y)
{
    setRectangle(x, y, width_, height_);
}

void LayoutElement::setSize(float width, float height)
{
    setRectangle(x_, y_, width, height);
}

void LayoutElement::addChild(std::shared_ptr<LayoutElement> child)
{
    // child->parent_ = shared_from_this();
    children_.emplace_back(child);
    updateLayout();
}

void LayoutElement::updateLayout()
{

}

void LayoutElement::setRectangle(float x, float y, float width, float height)
{
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;

    updateLayout();
}

}