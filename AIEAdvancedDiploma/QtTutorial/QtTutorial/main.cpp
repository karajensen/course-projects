////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <memory>
#include <mutex>
#include <iostream>
#include "QtWidgets/qapplication.h"

/**
* Cache item able to be locked across threads
*/
template <typename T> class Lockable
{
public:

    /**
    * Constructor
    */
    Lockable() :
        m_data(T())
    {
    }

    /**
    * Destructor
    */
    virtual ~Lockable()
    {
    }

    /**
    * Locks the thread to set the data
    * @param data The data to set
    */
    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
    }

    /**
    * Locks the thread to get the data
    * @return a copy of the data
    */
    T Get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

private:

    mutable std::mutex m_mutex;   ///< Mutex for access
    T m_data;                     ///< Internal data
};

/**
* Manages information sending between the gui and application
*/
struct Cache
{
    Lockable<bool> ApplicationRunning;
    Lockable<bool> ExampleButton;
    Lockable<float> ExampleSpinBox;
};

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