////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "glm\glm.hpp"

/**
* Main application class
*/
class Application
{
public:

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
    * Initialises OpenGL
    * @return whether or not initialisation succeeded
    */
    bool InitialiseOpenGL();

    /**
    * Initialises the scene
    * @return whether or not initialisation succeeded
    */
    bool InitialiseScene();

    /**
    * Renders the scene
    */
    void RenderScene();

    GLFWwindow* m_window = nullptr; ///< Handle to the application window
    glm::mat4 m_view;               ///< View matrix
    glm::mat4 m_projection;         ///< Projection matrix
};