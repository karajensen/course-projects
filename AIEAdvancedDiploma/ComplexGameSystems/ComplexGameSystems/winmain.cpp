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

    BlockingLockable<WindowHandle> handle;
    Lockable<bool> runApplication;
    Lockable<GuiRequestType> guiRequest;
    Lockable<int> guiRequestValue;
};

/**
* Runs GUI on a seperate thread to the main application
*/
void GuiMain(SharedData* data)
{
    auto gui = std::make_shared<GUI::GuiWrapper>();
    data->handle.Set(gui->GetWindowHandle());

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

    auto application = std::make_unique<Application>();
    if (!application->Initialize(data->handle.Get().handle))
    {
        return EXIT_FAILURE;
    }
    
    while(data->runApplication.Get())
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

    thread.join();

    #ifdef _DEBUG
    OutputDebugString("Exiting Simulation\n");
    #endif

    return EXIT_SUCCESS;
}