#pragma once

#include <win32_wrapper/layout/layout_element.hpp>


namespace win32w
{

class ContainerLayout : LayoutElement
{
private:
    float itemX_{0};
    float itemY_{0};
public:
    float itemWidth{0};
    float itemHeight{0};

    float itemX();
    float itemY();

    virtual void updateLayout() override;

};

}