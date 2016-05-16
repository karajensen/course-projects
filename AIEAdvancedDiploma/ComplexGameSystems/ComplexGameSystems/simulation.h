////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - simulation.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GUIcallbacks.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <memory>

class Camera;
class Timer;

/**
* Main Simulation Class
*/
class Simulation
{
public:

    /**
    * Constructor
    */
    Simulation();

    /**
    * Destructor
    */
    ~Simulation();

    /**
    * Initialises the simulation
    * @param hInstance The instance to the window
    * @param hWnd The handle to the window
    * @param d3ddev The directX device
    * @return whether initialisation succeeded
    */
    bool CreateSimulation(HINSTANCE hInstance, HWND hWnd, LPDIRECT3DDEVICE9 d3ddev);

    /**
    * Loads the gui callbacks
    * @param callbacks Methods the gui can call for the simulation
    */
    void LoadGuiCallbacks(GuiCallbacks* callbacks);

    /**
    * Renders the simulation
    */
    void Render();

    /**
    * Updates the simulation
    */
    void Update();

private:

    /**
    * Prevent copying
    */
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    std::unique_ptr<Timer> m_timer;              ///< Simulation timer object
    std::unique_ptr<Camera> m_camera;            ///< Main camera
    LPDIRECT3DDEVICE9 m_d3ddev;                  ///< DirectX device
};
