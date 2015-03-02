////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "opengl.h"
#include "input.h"
#include "camera.h"
#include "glfw/glfw3.h"
#include "aie/Gizmos.h"

Application::Application() :
    m_camera(std::make_unique<Camera>()),
    m_engine(std::make_unique<OpenGL>())
{
}

Application::~Application() = default;

void Application::Run()
{
    while(m_engine->IsRunning())
    {
        UpdateScene();

        m_engine->BeginRender();
        RenderScene();
        m_engine->EndRender();
    }
}

void Application::UpdateScene()
{
    const float currentTime = static_cast<float>(glfwGetTime());
    m_deltaTime = currentTime - m_previousTime;
    m_previousTime = currentTime;

    m_input->Update();
    m_camera->Update();
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

    Gizmos::draw(m_camera->ViewProjection());
}

void Application::Release()
{
    Gizmos::destroy();
    m_engine->Release();
}

bool Application::Initialise()
{
    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    InitialiseInput();

    Gizmos::create();
    return true;
}

void Application::InitialiseInput()
{
    m_input = std::make_unique<Input>(m_engine->GetWindow());
    m_input->AddCallback(GLFW_KEY_W, true, [this](){ m_camera->Forward(-m_deltaTime); });
    m_input->AddCallback(GLFW_KEY_S, true, [this](){ m_camera->Forward(m_deltaTime); });
    m_input->AddCallback(GLFW_KEY_D, true, [this](){ m_camera->Right(m_deltaTime); });
    m_input->AddCallback(GLFW_KEY_A, true, [this](){ m_camera->Right(-m_deltaTime); });
    m_input->AddCallback(GLFW_KEY_Q, true, [this](){ m_camera->Up(m_deltaTime); });
    m_input->AddCallback(GLFW_KEY_E, true, [this](){ m_camera->Up(-m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_LEFT_ALT, true, [this]()
    {
        if (m_input->IsMousePressed())
        {
            m_camera->Rotate(m_input->GetMouseDirection(), m_deltaTime);

        }
    });
}