#pragma once

#include <tuple>

namespace win32w
{

/// @brief A layout which distribute elements over equal sized cells.
class TiledLayout
{
public:
    int x {0};
    int y {0};
    int wItemSize{1};
    int hItemSize{1};
    int padding{0};

    std::tuple<float, float> computePosition(int c, int r);

};

}