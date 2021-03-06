////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "directxengine.h"
#include "vectorization.h"
#include "timer.h"
#include "opencv.h"
#include "tweaker.h"

Application::Application() = default;

Application::~Application() = default;

void Application::Close()
{
    m_openCV->Close();
    m_vectorization->Release();

    #ifdef _DEBUG
    OutputDebugString("Application::~Application\n");
    #endif
}

void Application::Render()
{
    m_timer->UpdateTimer();

    if(!m_paused)
    {
        if (m_openCV->Update())
        {
            if (m_vectorization->RequiresVectorization())
            {
                m_vectorization->Render(m_openCV->GetFrame());

                m_engine->GetContext()->CopyResource(
                    m_engine->GetBackBuffer(),
                    m_vectorization->GetBuffer());
            }
            else
            {
                m_engine->GetContext()->CopyResource(
                    m_engine->GetBackBuffer(),
                    m_openCV->GetFrame());
            } 

            if (m_diagnostics)
            {
                m_tweaker->Update();
                m_vectorization->SetActive();
            }
        }
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
    auto* device = m_engine->GetDevice();
    auto* context = m_engine->GetContext();

    m_openCV = std::make_unique<OpenCV>(device, context, size);
    if (!m_openCV->Initialize())
    {
        return false;
    }

    m_vectorization = std::make_unique<Vectorization>(device, context, size);
    if (!m_vectorization->Initialise(".//Resources//vectorization.fx"))
    {
        return false;
    }

    InitialiseTweakBar(size);

    m_vectorization->SetVectorization(0.0f);
    m_vectorization->SetActive();

    return true;
}

void Application::InitialiseTweakBar(const POINT& size)
{
    m_tweaker = std::make_unique<Tweaker>(m_engine->GetDevice(), size);

    m_timer->AddToTweaker(*m_tweaker);
    m_openCV->AddToTweaker(*m_tweaker);
    m_vectorization->AddToTweaker(*m_tweaker);
}

void Application::SetVectorizationAmount(float value)
{
    m_vectorization->SetVectorization(value);
}

void Application::TogglePause()
{
    m_paused = !m_paused;
}

void Application::ToggleDiagnostics()
{
    m_diagnostics = !m_diagnostics;
}

void Application::Save()
{
    m_engine->Save();
}