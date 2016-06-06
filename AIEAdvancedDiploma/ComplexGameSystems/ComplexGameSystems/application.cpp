////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "directxengine.h"
#include "timer.h"
#include "opencv.h"

Application::Application() = default;

Application::~Application() = default;

void Application::Close()
{
    m_openCV->Close();

    #ifdef _DEBUG
    OutputDebugString("Application::~Application\n");
    #endif
}

void Application::Render()
{
    m_timer->UpdateTimer();
    const float deltatime = m_timer->GetDeltaTime();

    if(!m_paused)
    {
        m_openCV->Update(m_vectorization);
    }

    m_engine->Render();
}

bool Application::Initialize(HWND hWnd, const POINT& size)
{   
    m_timer = std::make_unique<Timer>();
    m_timer->StartTimer();

    m_engine = std::make_unique<DirectxEngine>();
    if (!m_engine->Initialize(hWnd, size))
    {
        return false;
    }

    m_openCV = std::make_unique<OpenCV>(*m_engine, size.x, size.y);
    if (!m_openCV->Initialize())
    {
        return false;
    }

    return true;
}

void Application::SetVectorizationAmount(int value)
{
    m_vectorization = value;
}

void Application::TogglePause()
{
    m_paused = !m_paused;
}

void Application::Save()
{
    m_engine->Save();
}