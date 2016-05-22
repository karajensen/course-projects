////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

static const int NO_INDEX = -1;
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

/**
* Shows a popup message
*/
inline void ShowMessageBox(const std::string& message)
{
    MessageBox(NULL, message.c_str(), TEXT("ERROR"), MB_OK);
}

