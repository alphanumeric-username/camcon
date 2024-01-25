#pragma once

#include <memory>
#include <vector>

namespace win32w
{

class LayoutElement : std::enable_shared_from_this<LayoutElement>
{
protected:
    std::shared_ptr<LayoutElement> parent_{nullptr};
    std::vector<std::shared_ptr<LayoutElement>> children_{};
    float x_;
    float y_;
    float width_;
    float height_;

public:
    virtual void addChild(std::shared_ptr<LayoutElement> child);
    virtual void setPosition(float x, float y);
    virtual void setSize(float width, float height);
    virtual void setRectangle(float x, float y, float width, float height);
    virtual void updateLayout();
};

}