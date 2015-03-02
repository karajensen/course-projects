////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include "common.h"
#include "application.h"
#include "glfw/glfw3.h"

// Disables console window in release
#ifndef _DEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/**
* Main entry point
*/
int main()
{
    bool pauseConsole = true;

    auto application = std::make_unique<Application>();
    if (application->Initialise())
    {
        pauseConsole = false;
        application->Run();
    }

    application->Release();

    if (pauseConsole)
    {
        #ifdef _DEBUG
            std::cin.get();
        #endif
    }

    return 0;
}