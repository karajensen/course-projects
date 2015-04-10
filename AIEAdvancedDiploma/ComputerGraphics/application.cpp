////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "renderdata.h"
#include "opengl.h"
#include "input.h"
#include "camera.h"
#include "scene.h"
#include "gui.h"

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
        if (m_input->IsRightMouseDown())
        {
            m_camera->Rotate(m_input->GetMouseDirection(), m_deltaTime);
        }
       
        m_gui->Update(*m_input);
        m_scene->Tick(m_deltaTime, m_camera->Position());

        m_engine->RenderScene(m_timePassed);
        m_gui->Render();
        m_engine->EndRender();
    }
}

void Application::Release()
{
    // Release all openGL resources before terminating engine
    m_gui.reset();
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

    if (!m_scene->Initialise(m_camera->Position()))
    {
        LogError("Could not initialise scene");
        return false;
    }

    // Requires engine to be initialised
    m_gui = std::make_unique<Gui>(*m_scene);

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
   
}