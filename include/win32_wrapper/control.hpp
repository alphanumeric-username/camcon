#pragma once

#include <win32_wrapper/msg_callback.hpp>

#include <Windows.h>

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace win32w
{

enum ControlType
{
    BUTTON,
    EDIT,
    COMBO_BOX,
    TRACKBAR,
    LIST_BOX
};


/// @brief Represents a Windows Controls object
class Control
{
private:
    std::map<UINT, MsgCallback> callbackMap_{};

public:
    /// @brief Window handle to the object
    HWND hwnd;

    /// @brief Indicates the control created
    ControlType type;
    
    /// @brief Remove all items from a ComboBox
    void clearItems();

    /// @brief Adds a item to a ComboBox
    /// @param item The item display name
    void addItem(std::wstring item);
    
    /// @brief Adds a list of items to a ComboBox
    /// @param items The items vector
    void addItem(std::vector<std::wstring> items);

    /// @brief Gets the current selected item index
    /// @return The index of the selected item or CB_ERR if none is selected
    int getCurrentItemIndex();
    
    /// @brief Sets the current selected item index
    /// @param idx The index of the item to be selected
    void setCurrentItemIndex(int idx);
    
    /// @brief Defines the maximum and minimum values for a Trackbar
    /// @param min Minimum value
    /// @param max Maximum value
    void setRange(UINT min, UINT max);

    /// @brief Sets the position of the track in a Trackbar
    /// @param pos The desired position
    void setTrackPosition(int pos);

    /// @brief Gets the position of the track in a Trackbar
    /// @return The current position
    int getTrackPosition();

    /// @brief Get the text of the Control
    /// @return The Control text
    std::wstring getText();

    /// @brief Changes the text of the Control
    /// @param text The new text
    void setText(std::wstring text);

    /// @brief Sets control position and size
    void setRectangle(int x, int y, int width, int height);

    /// @brief Enables and disables the control
    /// @param enabled If `true`, enables the control. If `false` disables it
    void setEnabled(bool enabled);


    MsgCallback getCallback(UINT msg);

    void setCallback(UINT msg, MsgCallback callback);

    

};

}