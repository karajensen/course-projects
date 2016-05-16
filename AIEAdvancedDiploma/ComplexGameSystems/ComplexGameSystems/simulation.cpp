////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - simulation.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "simulation.h"
#include "camera.h"
#include "timer.h"
#include "common.h"

namespace
{
    const D3DCOLOR BACK_BUFFER_COLOR(D3DCOLOR_XRGB(190, 190, 195)); 
    const D3DCOLOR RENDER_COLOR(D3DCOLOR_XRGB(0, 0, 255));          
    const D3DCOLOR UPDATE_COLOR(D3DCOLOR_XRGB(0, 255, 0));          
}

Simulation::Simulation() :
    m_d3ddev(nullptr)
{
}

Simulation::~Simulation()
{
    #ifdef _DEBUG
    OutputDebugString("Simulation::~Simulation\n");
    #endif
}

void Simulation::Render()
{
    D3DPERF_BeginEvent(RENDER_COLOR, L"Simulation::Render");

    m_d3ddev->BeginScene();
    m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, BACK_BUFFER_COLOR, 1.0f, 0);
    m_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    m_d3ddev->EndScene();
    m_d3ddev->Present(nullptr, nullptr, nullptr, nullptr);

    D3DPERF_EndEvent();
}

void Simulation::Update()
{
    D3DPERF_BeginEvent(UPDATE_COLOR, L"Simulation::Update");

    m_timer->UpdateTimer();
    m_camera->UpdateCamera();

    const float deltatime = m_timer->GetDeltaTime();

    D3DPERF_EndEvent();
}

void Simulation::LoadGuiCallbacks(GuiCallbacks* callbacks)
{
    callbacks->setVectorizationAmount = 
        [](int amount) { ShowMessageBox(std::to_string(amount)); };
}

bool Simulation::CreateSimulation(HINSTANCE hInstance, HWND hWnd, LPDIRECT3DDEVICE9 d3ddev) 
{   
    m_d3ddev = d3ddev;

    const D3DXVECTOR3 position(0.0f, 0.0f, -30.0f);
    const D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    m_camera = std::make_unique<Camera>(position, target);
    m_camera->CreateProjectionMatrix();

    m_timer = std::make_unique<Timer>();
    m_timer->StartTimer();

    return true;
}