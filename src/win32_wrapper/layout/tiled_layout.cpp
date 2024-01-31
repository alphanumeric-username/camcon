#include <win32_wrapper/layout/tiled_layout.hpp>

#include <cmath>

namespace win32w
{


float TiledLayout::itemX(int c) { return x_ + c * (itemWidth + padding); }

float TiledLayout::itemY(int r) { return y_ + r * (itemHeight + padding); }

void TiledLayout::updateLayout()
{
    if(children_.empty())
    {
        return;
    }

    int rows = std::ceil(1.0*children_.size()/columns);
    
    float totalWidth = columns * itemWidth + (columns - 1) * padding;
    float totalHeight = rows * itemHeight + (rows - 1) * padding;
    float cx = (x_ + width_/2);
    float cy = (y_ + height_/2);
    float x0 = cx - totalWidth/2;
    float y0 = cy - totalHeight/2;
    

    for(int c = 0; c < columns; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            int i = c + r * columns;
            if(i < children_.size())
            {
                // float idx { x_ + c * (itemWidth + padding) };
                // float idy { y_ + r * (itemHeight + padding) };
                float idx { 1.0f * c * (itemWidth + padding) };
                float idy { 1.0f * r * (itemHeight + padding) };

                auto ix { x0 + idx };
                auto iy { y0 + idy };

                children_[i]->setRectangle(ix, iy, itemWidth, itemHeight);
            } else 
            {
                return;
            }
        }
    }

}


}