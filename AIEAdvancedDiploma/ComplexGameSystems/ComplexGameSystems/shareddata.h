////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shareddata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "lockable.h"
#include "GUITypes.h"

/**
* Data shared between threads which needs to be synchronized
*/
struct SharedData
{
    SharedData() :
        guiRequest(NONE),
        runApplication(true),
        guiRequestValue(0)
    {
    }

    void SendGuiValueRequest(GuiRequestType type, int value)
    {
        guiRequest.Set(type);
        guiRequestValue.Set(value);
    }

    void SendGuiRequest(GuiRequestType type)
    {
        guiRequest.Set(type);
    }

    void CloseApplication()
    {
        runApplication.Set(false);
#ifdef _DEBUG
        OutputDebugString("Application close requested\n");
#endif
    }

    void SendWindowHandle(HWND hwnd, HINSTANCE hinstance)
    {
        guiHandle.Set(hwnd);
        guiInstance.Set(hinstance);
    }

    void SendWindowSize(int x, int y)
    {
        POINT point;
        point.x = x;
        point.y = y;
        windowSize.Set(point);
    }

    BlockingLockable<HINSTANCE> guiInstance;
    BlockingLockable<HWND> guiHandle;
    BlockingLockable<POINT> windowSize;
    Lockable<bool> runApplication;
    Lockable<GuiRequestType> guiRequest;
    Lockable<int> guiRequestValue;
};