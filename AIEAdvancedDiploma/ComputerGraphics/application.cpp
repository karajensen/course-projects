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
#include "timer.h"

Application::Application() :
    m_camera(std::make_unique<Camera>()),
    m_scene(std::make_unique<Scene>()),
    m_engine(std::make_unique<OpenGL>(*m_scene, *m_camera)),
    m_timer(std::make_unique<Timer>())
{
}

Application::~Application() = default;

void Application::Run()
{
    while(m_engine->IsRunning())
    {
        m_timer->StartSection(Timer::SCENE_UPDATE);

        m_timer->UpdateTimer();
        const float deltaTime = m_timer->GetDeltaTime();
        const float timePassed = m_timer->GetTotalTime();

        m_input->Update();       
        m_gui->Update(*m_input);
        m_scene->Tick(deltaTime, *m_camera);

        m_camera->Update(m_input->GetMouseDirection(),
            m_input->IsRightMouseDown(), deltaTime);

        m_timer->StopSection(Timer::SCENE_UPDATE);
        m_timer->StartSection(Timer::RENDERING);

        m_engine->RenderScene(*m_timer, timePassed);
        m_gui->Render();
        m_engine->EndRender();

        m_timer->StopSection(Timer::RENDERING);
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

    InitialiseInput();

    // Requires application to be fully initialiseds
    m_gui = std::make_unique<Gui>(*m_scene, *m_camera, *m_input, *m_timer,
        std::bind(&OpenGL::ToggleWireframe, m_engine.get()));

    return true;
}

void Application::InitialiseInput()
{
    m_input = std::make_unique<Input>(m_engine->GetWindow());

    m_input->AddCallback(GLFW_KEY_F1, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::FINAL_MAP); });

    m_input->AddCallback(GLFW_KEY_F2, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::SCENE_MAP); });

    m_input->AddCallback(GLFW_KEY_F3, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::DEPTH_MAP); });

    m_input->AddCallback(GLFW_KEY_F4, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::BLUR_MAP); });

    m_input->AddCallback(GLFW_KEY_F5, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::BLOOM_MAP); });

    m_input->AddCallback(GLFW_KEY_F6, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::FOG_MAP); });

    m_input->AddCallback(GLFW_KEY_F7, false, 
        [this](){ m_scene->SetPostMap(PostProcessing::DOF_MAP); });

    m_input->AddCallback(GLFW_KEY_W, true, 
        [this](){ m_camera->Forward(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_S, true, 
        [this](){ m_camera->Forward(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_D, true, 
        [this](){ m_camera->Right(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_A, true, 
        [this](){ m_camera->Right(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_Q, true,
        [this](){ m_camera->Up(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_E, true, 
        [this](){ m_camera->Up(-m_timer->GetDeltaTime()); });
   
}