////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class OpenGL;

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
    * Renders the scene
    */
    void RenderScene();

    std::unique_ptr<OpenGL> m_engine; ///< Render engine to use for application
};