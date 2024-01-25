#pragma once

#include <win32_wrapper/control.hpp>
#include <win32_wrapper/gdi/gdi_text.hpp>
#include <win32_wrapper/msg_callback.hpp>

#include <Windows.h>

#include <vector>
#include <map>
#include <memory>

namespace win32w
{

/// @brief Represents a Window
class Window
{
private:
    /// @brief List of attached Controls.
    std::vector<std::shared_ptr<Control>> controlList_{};

    /// @brief List of attached GDITexts.
    std::vector<std::shared_ptr<GDIText>> gdiTextList_{};
    
    /// @brief Message callbacks.
    std::map<UINT, MsgCallback> callbackMap_{};

public:
    /// @brief Handle to the window.
    HWND hwnd{};

    /// @brief Adds a Control to the Window.
    /// @param ctrl A pointer to the Control
    void addControl(std::shared_ptr<Control> ctrl);

    /// @brief Adds a GDIText to the Window.
    /// @param ctrl A pointer to the GDIText
    void addGDIText(std::shared_ptr<GDIText> gdiText);

    /// @brief Adds a message callback to handle the specified message.
    /// @param uMsg Message code
    /// @param callback The callback. Must satisfy the MsgCallback type.
    void setCallback(UINT uMsg, MsgCallback callback);

    std::vector<std::shared_ptr<Control>> getControlList();
    std::vector<std::shared_ptr<GDIText>> getGdiTextList();

    void enableControls(bool enable = true);

    /// @brief Returns a callback to a message code.
    /// @param uMsg The message code
    /// @return If there is a callback for the specified message code, it is returned. Otherwise a empty function is returned.
    MsgCallback getCallback(UINT uMsg);


};

}