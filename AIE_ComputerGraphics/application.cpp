////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "aie\Gizmos.h"
#include "glm\ext.hpp"
#include <sstream>

void Application::Run()
{
    while(!glfwWindowShouldClose(m_window) && 
          glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderScene();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::RenderScene()
{
    Gizmos::clear();

    Gizmos::addTransform(glm::mat4(1));

    glm::vec4 white(1);
    glm::vec4 black(0,0,0,1);
    for(int i = 0; i < 21; ++i) 
    {
        Gizmos::addLine(
            glm::vec3(-10 + i, 0, 10),
            glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
    
        Gizmos::addLine(
            glm::vec3(10, 0, -10 + i), 
            glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }

    Gizmos::draw(m_projection * m_view);
}

void Application::Release()
{
    if (m_window)
    {
        Gizmos::destroy();
        glfwDestroyWindow(m_window);
    }
}

bool Application::Initialise()
{
    return InitialiseOpenGL() && InitialiseScene();
}

bool Application::InitialiseOpenGL()
{
    m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
    if (!m_window)
    {
        LogError("Could not create window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) 
    {
        LogError("OGL Load Failed");
        return false;
    }

    glEnable(GL_DEPTH_TEST); 
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    std::stringstream stream;
    stream << "Initialised OpenGL " << ogl_GetMajorVersion() 
        << "." << ogl_GetMinorVersion();
    LogInfo(stream.str());

    return true;
}

bool Application::InitialiseScene()
{
    Gizmos::create();

    m_view = glm::lookAt(glm::vec3(10,10,10), 
        glm::vec3(0), glm::vec3(0,1,0));

    m_projection = glm::perspective(
        glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);

    return true;
}