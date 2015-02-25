////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "glm\glm.hpp"

class GLFWwindow;

/**
* Engine for initialising and managing OpenGL
*/
class OpenGL
{
public:

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
    * @return the view projection matrix
    */
    const glm::mat4& GetViewProjection() const;

private: 

    /**
    * OpenGL call checking
    * @return whether the last call to OpenGL has failed
    * @note requires an OpenGL context to be created
    */
    bool HasCallFailed() const;

    GLFWwindow* m_window = nullptr; ///< Handle to the application window
    glm::mat4 m_view;               ///< View matrix
    glm::mat4 m_projection;         ///< Projection matrix
    glm::mat4 m_viewProjection;     ///< Combined view projection matrix
};