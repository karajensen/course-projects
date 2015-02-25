////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl.h"
#include "opengl/gl_core_4_4.h"
#include "glfw/glfw3.h"
#include "glm/ext.hpp"
#include <sstream>

bool OpenGL::IsRunning() const
{
    return !glfwWindowShouldClose(m_window) && 
          glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void OpenGL::Release()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void OpenGL::BeginRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_viewProjection = m_projection * m_view;
}

void OpenGL::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

const glm::mat4& OpenGL::GetViewProjection() const
{
    return m_viewProjection;
}

bool OpenGL::Initialise()
{
    if (!glfwInit())
    {
        LogError("OpenGL: Could not initialize GLFW");
        return false;
    }

    m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
    if (!m_window)
    {
        LogError("OpenGL: Could not create window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) 
    {
        LogError("OpenGL: OGL Load Failed");
        return false;
    }

    glEnable(GL_DEPTH_TEST); 
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    m_view = glm::lookAt(glm::vec3(10,10,10), 
        glm::vec3(0), glm::vec3(0,1,0));

    m_projection = glm::perspective(
        glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

    std::stringstream stream;
    stream << "OpenGL: Initialised " << ogl_GetMajorVersion() 
        << "." << ogl_GetMinorVersion();
    LogInfo(stream.str());

    return !HasCallFailed();
}

bool OpenGL::HasCallFailed() const
{
    switch(glGetError())
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_VALUE:
        LogError("OpenGL: Invalid Value");
        return true;
    case GL_INVALID_OPERATION:
        LogError("OpenGL: Invalid Operation");
        return true;
    default:
        LogError("OpenGL: Unknown Error");
        return true;
    }
}