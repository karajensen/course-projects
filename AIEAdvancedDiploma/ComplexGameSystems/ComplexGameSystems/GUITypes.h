////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIcallbacks.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <utility>
#include <functional>

/**
* GUI request type
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
    std::function<void(GuiRequestType, float)> sendValueRequest;
    std::function<void(GuiRequestType)> sendRequest;
    std::function<void(void)> closeApplication;
    std::function<void(HWND, HINSTANCE)> sendWindowHandle;
    std::function<void(int, int)> sendWindowSize;
};