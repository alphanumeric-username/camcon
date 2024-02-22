#include <win32_wrapper/event_callback.hpp>

#include <win32_wrapper/window_registry.hpp>

#include <CommCtrl.h>

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

    // } else if (uMsg == WM_ERASEBKGND) {

    } else if (uMsg == WM_COMMAND || uMsg == WM_NOTIFY || uMsg == WM_VSCROLL)
    {
        UINT eventCode {0};
        
        eventCode = uMsg == WM_COMMAND ? HIWORD(wParam) : 
                    uMsg == WM_NOTIFY ? ((NMHDR*) lParam)->code
                                      : WM_VSCROLL;

        for(auto c : window->getControlList())
        {
            if(c->hwnd == reinterpret_cast<HWND>(lParam))
            {
                c->getCallback(eventCode)(parentHwnd, uMsg, wParam, lParam);
                break;
            }
        }
    }

    window->getCallback(uMsg)(parentHwnd, uMsg, wParam, lParam);

    return DefWindowProc(parentHwnd, uMsg, wParam, lParam);
}

}