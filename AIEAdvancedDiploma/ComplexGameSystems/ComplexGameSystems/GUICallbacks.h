////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIcallbacks.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <functional>

/**
* Callbacks for use in the GUI
*/
struct GuiCallbacks
{
    std::function<void(int)> setVectorizationAmount = nullptr;
    std::function<void(void)> quitFn = nullptr;
};

/**
* GUI window information
*/
struct WindowHandle
{
    HWND handle = nullptr;
    HINSTANCE instance = nullptr;
};