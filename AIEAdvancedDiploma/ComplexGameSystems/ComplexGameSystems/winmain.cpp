////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - winmain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "application.h"
#include "GUITypes.h"
#include "GUIWrapper.h"
#include "lockable.h"
#include <thread>
#include <mutex>

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

    BlockingLockable<WindowHandle> guiHandle;
    Lockable<bool> runApplication;
    Lockable<GuiRequestType> guiRequest;
    Lockable<int> guiRequestValue;
};

/**
* Main application window message handler
* Exit is handled through the GUI thread
*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
* Initialises the main window
*/
HWND InitializeWindow(HINSTANCE& hInstance, HWND& parent)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "ComplexGameSystems";
    RegisterClassEx(&wc);

    HWND handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        "ComplexGameSystems", TEXT("ComplexGameSystems"),
        WS_CHILD, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        parent, nullptr, hInstance, nullptr);

    if (handle == nullptr)
    {
        ShowMessageBox("Failed to create application window");
    }

    ShowWindow(handle, SW_SHOWDEFAULT);

    return handle;
}

/**
* Runs GUI on a seperate thread to the main application
*/
void GuiMain(SharedData* data)
{
    auto gui = std::make_shared<GUI::GuiWrapper>();
    data->guiHandle.Set(gui->GetWindowHandle());

    GuiRequestCallbacks requests;
    requests.sendRequest = [data](GuiRequestType type)
    {
        data->guiRequest.Set(type);
    };
    requests.sendValueRequest = [data](GuiRequestType type, int value)
    {
        data->guiRequest.Set(type);
        data->guiRequestValue.Set(value);
    };
    requests.closeApplication = [data]()
    {
        data->runApplication.Set(false);
    };

    gui->Initialize(&requests);
    gui->Show();

    while (data->runApplication.Get())
    {
        gui->Update();
    }
}

/**
* Main application entry point
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    auto data = std::make_unique<SharedData>();

    std::thread thread(&GuiMain, data.get());

    HWND guiHandle = data->guiHandle.Get().handle;
    HWND appHandle = InitializeWindow(hInstance, guiHandle);
    
    auto application = std::make_unique<Application>();
    if (!application->Initialize(appHandle))
    {
        return EXIT_FAILURE;
    }
    
    while(data->runApplication.Get())
    {
        MSG msg;
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            switch (data->guiRequest.Swap(NONE))
            {
            case PAUSE:
                application->TogglePause();
                break;
            case SAVE:
                application->Save();
                break;
            case VECTORIZATION:
                application->SetVectorizationAmount(data->guiRequestValue.Get());
                break;
            }
            application->Render();
        }
    }

    SetParent(appHandle, 0);
    thread.join();

    #ifdef _DEBUG
    OutputDebugString("Exiting Simulation\n");
    #endif

    return EXIT_SUCCESS;
}