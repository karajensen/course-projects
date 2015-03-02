////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

class OpenGL;
class Camera;
class Input;

/**
* Main application class
*/
class Application
{
public:

    Application();
    ~Application();

    /**
    * Main loop for the application
    */
    void Run();

    /**
    * Initialise the world
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * Releases the world
    */
    void Release();

private: 

    /**
    * Prevent copying
    */
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
    * Renders the scene
    */
    void RenderScene();

    /**
    * Updates the scene
    */
    void UpdateScene();

    /**
    * Initialises the input for the application
    */
    void InitialiseInput();

    float m_previousTime = 0.0f;      ///< Time previously saved last tick
    float m_deltaTime = 0.0f;         ///< Time passed between ticks
    std::unique_ptr<OpenGL> m_engine; ///< Render engine to use for application
    std::unique_ptr<Camera> m_camera; ///< The camera viewing the scene
    std::unique_ptr<Input> m_input;   ///< Manages input from the window
};