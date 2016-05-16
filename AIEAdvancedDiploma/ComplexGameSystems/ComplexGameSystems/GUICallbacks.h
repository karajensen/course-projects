////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIcallbacks.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <functional>

typedef std::function<void(void)> VoidFn;

/**
* Callbacks for use in the GUI
*/
struct GuiCallbacks
{
    VoidFn testButton;
    VoidFn quitFn;
};

/**
* GUI window information
*/
struct WindowHandle
{
    HWND handle;
    HINSTANCE instance;
};