////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "cache.h"
#include <thread>
#include <memory>
#include <iostream>
#include "QtWidgets/qapplication.h"

/**
* Runs Qt on a seperate thread to the main application
*/
void qtmain(int argc, char *argv[], std::shared_ptr<Cache> cache)
{
    QApplication app(argc, argv);

    // Qt GUI loop is here
    while(cache->ApplicationRunning.Get())
    {
        app.processEvents();
    }

    app.exit();
}

/**
* Application entry point
*/
int main(int argc, char *argv[])
{
    // Initialise the shared cache between the GUI and application
    auto cache = std::make_shared<Cache>();
    cache->ApplicationRunning.Set(true);

    // GUI will run on a seperate thread
    std::thread thread(&qtmain, argc, argv, cache);

    // Application loop is here
    while (cache->ApplicationRunning.Get())
    {
        if (cache->ExampleButton.Get())
        {
            std::cout << "Example Button was pressed!" << std::endl;
            cache->ExampleButton.Set(false);
        }
    }

    thread.join();
    return 0;
}