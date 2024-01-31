#include <win32_wrapper/layout/layout_manager.hpp>

namespace win32w
{

bool win32w::LayoutManager::stateIsValid() const
{
    return root != nullptr & win != nullptr;
}

void win32w::LayoutManager::update()
{
    if(!stateIsValid())
    {
        return;
    }

    auto winRect { win->rect() };
    root->setRectangle(0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top);
    win->repaint();
}


}