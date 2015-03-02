////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"

struct GLFWwindow;

/**
* Engine for initialising and managing OpenGL
*/
class OpenGL
{
public:

    /**
    * Constructor
    */
    OpenGL() = default;

    /**
    * Initialise OpenGL
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * Releases OpenGL
    */
    void Release();

    /**
    * @return whether OpenGL is currently running
    */
    bool IsRunning() const;

    /**
    * Starts rendering OpenGL
    */
    void BeginRender();

    /**
    * Ends rendering OpenGL
    */
    void EndRender();

    /**
    * @return the application window
    */
    GLFWwindow& GetWindow() const;

private: 

    /**
    * Prevent copying
    */
    OpenGL(const OpenGL&) = delete;
    OpenGL& operator=(const OpenGL&) = delete;

    /**
    * OpenGL call checking
    * @return whether the last call to OpenGL has failed
    * @note requires an OpenGL context to be created
    */
    bool HasCallFailed() const;

    GLFWwindow* m_window = nullptr; ///< Handle to the application window
};