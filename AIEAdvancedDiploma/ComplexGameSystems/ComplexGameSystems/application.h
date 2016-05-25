////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <memory>
#include <Windows.h>

class Timer;
class DirectxEngine;

/**
* Main Application Class
*/
class Application
{
public:

    /**
    * Constructor
    */
    Application();

    /**
    * Destructor
    */
    ~Application();

    /**
    * Initialises the application
    * @param hWnd The handle to the window
    * @param size The size of the window
    * @return whether initialisation succeeded
    */
    bool Initialize(HWND hWnd, const POINT& size);

    /**
    * Renders the application
    */
    void Render();

    /**
    * Sets the amount of vectorization
    */
    void SetVectorizationAmount(int value);

    /**
    * Toggles whether the application is paused
    */
    void TogglePause();

    /**
    * Saves the current image
    */
    void Save();

private:

    /**
    * Prevent copying
    */
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int m_vectorization = 0;
    bool m_paused = false;
    std::unique_ptr<Timer> m_timer; 
    std::unique_ptr<DirectxEngine> m_engine;
};
