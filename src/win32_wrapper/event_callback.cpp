#include <win32_wrapper/event_callback.hpp>

#include <win32_wrapper/window_registry.hpp>

namespace win32w
{

LRESULT eventCallback(HWND parentHwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto window = window_registry::getWindow(parentHwnd);
    if(window == nullptr)
    {
        return DefWindowProc(parentHwnd, uMsg, wParam, lParam);
    }

    if(uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    } else if (uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(parentHwnd, &ps);
            FillRect(hdc, &(ps.rcPaint), (HBRUSH) (COLOR_WINDOW+1));
            
            for(auto t : window->getGdiTextList())
            {
                auto contents = t->contents;
                t->onPaint(hdc);
            }

        EndPaint(parentHwnd, &ps);

        return 0;
    } else if (uMsg == WM_COMMAND)
    {
        auto eventCode = HIWORD(wParam);
        if(eventCode == BN_CLICKED)
        {
            for(auto c : window->getControlList())
            {
                if(c->hwnd == reinterpret_cast<HWND>(lParam))
                {
                    c->onClick(parentHwnd);
                }
            }
        } else if (eventCode == CBN_SELCHANGE)
        {
            for(auto c : window->getControlList())
            {
                if(c->hwnd == reinterpret_cast<HWND>(lParam))
                {
                    c->onSelect(parentHwnd);
                }
            }
        }

        return 0;
    } else {
        window->getCallback(uMsg)(parentHwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(parentHwnd, uMsg, wParam, lParam);
}

}