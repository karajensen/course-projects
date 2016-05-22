////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIcallbacks.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <utility>
#include <functional>

/**
* GUI requests
*/
enum GuiRequestType
{
    NONE,
    PAUSE,
    SAVE,
    VECTORIZATION
};

/**
* GUI request callbacks
*/
struct GuiRequestCallbacks
{
    std::function<void(GuiRequestType, int)> sendValueRequest;
    std::function<void(GuiRequestType)> sendRequest;
    std::function<void(void)> closeApplication;
};

/**9
* GUI window information
*/
struct WindowHandle
{
    bool IsInitialized() const
    {
        return handle != nullptr && instance != nullptr;
    }

    HWND handle = nullptr;
    HINSTANCE instance = nullptr;
};