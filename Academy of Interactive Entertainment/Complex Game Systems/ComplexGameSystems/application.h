////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <memory>
#include <Windows.h>

class Timer;
class DirectxEngine;
class OpenCV;
class Vectorization;
class Tweaker;

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
    * Closes the application
    */
    void Close();

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
    void SetVectorizationAmount(float value);

    /**
    * Toggles whether the application is paused
    */
    void TogglePause();

    /**
    * Toggles whether diagnostics are on
    */
    void ToggleDiagnostics();

    /**
    * Saves the current image
    */
    void Save();

private:

    /**
    * Initialises the tweak bar
    */
    void InitialiseTweakBar(const POINT& point);

    /**
    * Prevent copying
    */
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool m_diagnostics = false;
    bool m_paused = false;
    std::unique_ptr<Timer> m_timer; 
    std::unique_ptr<DirectxEngine> m_engine;
    std::unique_ptr<OpenCV> m_openCV;
    std::unique_ptr<Vectorization> m_vectorization;
    std::unique_ptr<Tweaker> m_tweaker;
};
