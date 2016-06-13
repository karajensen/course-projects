////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - winmain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <thread>
#include "application.h"
#include "GUIWrapper.h"
#include "shareddata.h"

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
HWND InitializeWindow(HINSTANCE& hInstance, HWND& parent, const POINT& size)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "Child";
    RegisterClassEx(&wc);

    HWND handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        wc.lpszClassName, wc.lpszClassName, WS_CHILD, 0, 0, 
        size.x, size.y, parent, nullptr, hInstance, nullptr);

    if (handle == nullptr)
    {
        MessageBox(0, "Failed to create application window", "ERROR", MB_OK);
    }

    ShowWindow(handle, SW_SHOWDEFAULT);

    return handle;
}

/**
* Runs GUI on a seperate thread to the main application
*/
void GuiMain(SharedData* data)
{
    using namespace std::placeholders;

    auto gui = std::make_shared<GUI::GuiWrapper>();

    GuiRequestCallbacks requests;
    requests.sendRequest = std::bind(&SharedData::SendGuiRequest, data, _1);
    requests.sendValueRequest = std::bind(&SharedData::SendGuiValueRequest, data, _1, _2);
    requests.closeApplication = std::bind(&SharedData::CloseApplication, data);
    requests.sendWindowHandle = std::bind(&SharedData::SendWindowHandle, data, _1, _2);
    requests.sendWindowSize = std::bind(&SharedData::SendWindowSize, data, _1, _2);

    gui->Initialize(&requests);
    gui->Show();

    while (data->runApplication.Get())
    {
        gui->Update();
    }

    #ifdef _DEBUG
    OutputDebugString("Closing GUI thread\n");
    #endif
}

/**
* Main application entry point
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    auto data = std::make_unique<SharedData>();

    std::thread thread(&GuiMain, data.get());

    POINT windowSize = data->windowSize.Get(); // Blocks
    HWND guiHandle = data->guiHandle.Get(); // Blocks
    HWND appHandle = InitializeWindow(hInstance, guiHandle, windowSize);
    
    auto application = std::make_unique<Application>();
    if (!application->Initialize(appHandle, windowSize))
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
            case BORDER:
                application->ToggleBorder();
                break;
            }
            application->Render();
        }
    }

    application->Close();

    SetParent(appHandle, 0);
    ::DestroyWindow(appHandle);
    ::UnregisterClass("Child", hInstance);

    thread.join();

    #ifdef _DEBUG
    OutputDebugString("Exiting Simulation\n");
    #endif

    return EXIT_SUCCESS;
}