////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "renderdata.h"
#include "opengl.h"
#include "input.h"
#include "camera.h"
#include "scene.h"

namespace
{
    const float DT_MAXIMUM = 0.03f;   ///< Maximum allowed deltatime
    const float DT_MINIMUM = 0.01f;   ///< Minimum allowed deltatime
}

Application::Application() :
    m_camera(std::make_unique<Camera>()),
    m_scene(std::make_unique<Scene>()),
    m_engine(std::make_unique<OpenGL>(*m_scene, *m_camera))
{
}

Application::~Application() = default;

void Application::Run()
{
    while(m_engine->IsRunning())
    {
        const float currentTime = static_cast<float>(glfwGetTime());
        m_deltaTime = Clamp(currentTime - m_previousTime, DT_MINIMUM, DT_MAXIMUM);
        m_previousTime = currentTime;
        m_timePassed += m_deltaTime;

        m_input->Update();
        m_camera->Update();
        m_scene->Tick(m_deltaTime);
        m_engine->RenderScene(m_timePassed);
    }
}

void Application::Release()
{
    // Release all openGL resources before terminating engine
    m_scene.reset();
    m_engine.reset();
}

bool Application::Initialise()
{
    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    if (!m_scene->Initialise())
    {
        LogError("Could not initialise scene");
        return false;
    }

    InitialiseInput();

    return true;
}

void Application::InitialiseInput()
{
    m_input = std::make_unique<Input>(m_engine->GetWindow());

    m_input->AddCallback(GLFW_KEY_1, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::FINAL_MAP); });

    m_input->AddCallback(GLFW_KEY_2, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::SCENE_MAP); });

    m_input->AddCallback(GLFW_KEY_3, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::NORMAL_MAP); });

    m_input->AddCallback(GLFW_KEY_4, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::DEPTH_MAP); });

    m_input->AddCallback(GLFW_KEY_5, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::BLUR_MAP); });

    m_input->AddCallback(GLFW_KEY_6, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::BLOOM_MAP); });

    m_input->AddCallback(GLFW_KEY_7, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::FOG_MAP); });

    m_input->AddCallback(GLFW_KEY_8, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::DOF_MAP); });

    m_input->AddCallback(GLFW_KEY_0, false, 
        [this](){ m_engine->ToggleWireframe(); });

    m_input->AddCallback(GLFW_KEY_P, false, 
        [this](){ m_scene->SaveTextures(); });

    m_input->AddCallback(GLFW_KEY_O, false, 
        [this](){ m_scene->Reload(); });

    m_input->AddCallback(GLFW_KEY_W, true, 
        [this](){ m_camera->Forward(-m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_S, true, 
        [this](){ m_camera->Forward(m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_D, true, 
        [this](){ m_camera->Right(m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_A, true, 
        [this](){ m_camera->Right(-m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_Q, true,
        [this](){ m_camera->Up(m_deltaTime); });

    m_input->AddCallback(GLFW_KEY_E, true, 
        [this](){ m_camera->Up(-m_deltaTime); });
    
    m_input->AddMouseCallback([this]()
    {
        m_camera->Rotate(m_input->GetMouseDirection(), m_deltaTime);
    });
}