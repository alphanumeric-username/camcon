#pragma once

#include <win32_wrapper/layout/layout_element.hpp>

#include <tuple>

namespace win32w
{

/// @brief A layout which distribute elements over equal sized cells.
class TiledLayout : public LayoutElement
{
public:
    // int x {0};
    // int y {0};
    int itemWidth{1};
    int itemHeight{1};
    int columns {1};
    int padding{0};

    float itemX(int c);
    float itemY(int r);
    virtual void updateLayout() override;


};

}