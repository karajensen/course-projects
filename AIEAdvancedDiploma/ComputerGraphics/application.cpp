////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "opengl.h"
#include "aie\Gizmos.h"

Application::Application() = default;
Application::~Application() = default;

void Application::Run()
{
    while(m_engine->IsRunning())
    {
        m_engine->BeginRender();

        RenderScene();

        m_engine->EndRender();
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

    Gizmos::draw(m_engine->GetViewProjection());
}

void Application::Release()
{
    Gizmos::destroy();
    m_engine->Release();
}

bool Application::Initialise()
{
    m_engine = std::make_unique<OpenGL>();
    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    Gizmos::create();
    return true;
}