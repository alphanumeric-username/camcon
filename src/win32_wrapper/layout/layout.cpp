#include <win32_wrapper/layout/tiled_layout.hpp>

namespace win32w
{

std::tuple<float, float> TiledLayout::computePosition(int c, int r)
{
    int posx {
        x + c * (wItemSize + padding),
    };
    int posy {
        y + r * (hItemSize + padding),
    };

    return {posx, posy};
}

}