////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <memory>
#include <iostream>
#include "qt/form.h"
#include "QtWidgets/qapplication.h"
#include "cache.h"

/**
* Runs Qt on a seperate thread to the main application
*/
void qtmain(int argc, char *argv[], std::shared_ptr<Cache> cache)
{
    QApplication app(argc, argv);
    Form form(cache);
    form.show();

    // Qt GUI loop is here
    while(cache->ApplicationRunning.Get())
    {
        app.processEvents();

        // When the form is closed exit the application
        if (!form.isVisible())
        {
            cache->ApplicationRunning.Set(false);
        }
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
        // When the button is pressed display the spin box value
        if (cache->ExampleButton.Get())
        {
            std::cout << cache->ExampleSpinBox.Get() << std::endl;
            cache->ExampleButton.Set(false);
        }
    }

    thread.join();
    return 0;
}