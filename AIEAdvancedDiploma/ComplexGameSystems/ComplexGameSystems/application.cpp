////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "directxengine.h"
#include "vectorization.h"
#include "timer.h"
#include "opencv.h"

// Uncomment to test without compute shader
//#define NO_VECTORIZATION

// Uncomment to show diagnostics
#define DIAGNOSTICS

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
    #ifdef DIAGNOSTICS
    m_timer->UpdateTimer();
    #endif

    if(!m_paused)
    {
        if (m_openCV->Update())
        {
            #ifdef DIAGNOSTICS
            m_openCV->RenderDiagnostics(
                m_timer->GetDeltaTime(),
                m_timer->GetFPS(),
                m_vectorization->GetVectorization());
            #endif

            #ifdef NO_VECTORIZATION
            m_engine->GetContext()->CopyResource(
                m_engine->GetBackBuffer(),
                m_openCV->GetFrame());
            #else   
            m_vectorization->CopyToBuffer(m_openCV->GetFrame());
            m_vectorization->Render();

            m_engine->GetContext()->CopyResource(
                m_engine->GetBackBuffer(),
                m_vectorization->GetBuffer());
            #endif
        }
    }

    m_engine->Render();
}

bool Application::Initialize(HWND hWnd, const POINT& size)
{   
    #ifdef _DEBUG
    m_timer = std::make_unique<Timer>();
    m_timer->StartTimer();
    #endif

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

    m_vectorization = std::make_unique<Vectorization>();
    if (!m_vectorization->Initialise(m_engine->GetDevice(),
                                     m_engine->GetContext(),
                                     ".//Resources//vectorization.fx", 
                                     size))
    {
        return false;
    }

    SetVectorizationAmount(0.0f);

    return true;
}

void Application::SetVectorizationAmount(float value)
{
    m_vectorization->SetVectorization(value);
}

void Application::TogglePause()
{
    m_paused = !m_paused;
}

void Application::Save()
{
    m_engine->Save();
}