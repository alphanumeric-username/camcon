#include <win32_wrapper/event_callback.hpp>

#include <win32_wrapper/window_registry.hpp>

namespace win32w
{

LRESULT eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto window = window_registry::getWindow(hwnd);
    if(window == nullptr)
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    if(uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    } else if (uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &(ps.rcPaint), (HBRUSH) (COLOR_WINDOW+1));
            
            for(auto t : window->getGdiTextList())
            {
                auto contents = t->contents;
                t->onPaint(hdc);
            }

        EndPaint(hwnd, &ps);

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
                    c->onClick(hwnd);
                }
            }
        }

        return 0;
    } else {
        window->getCallback(uMsg)(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

}