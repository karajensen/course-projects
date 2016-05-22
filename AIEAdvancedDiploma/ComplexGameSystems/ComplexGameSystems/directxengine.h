////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include <memory>

struct DirectxData;
struct ID3D11Device;

/**
* DirectX Graphics engine
*/
class DirectxEngine
{
public:

    /**
    * Constructor
    * @param hwnd Handle to the window
    */
    DirectxEngine();

    /**
    * Destructor
    */
    ~DirectxEngine();

    /**
    * Sets up the graphics engine for rendering
    * @return whether initialization succeeded
    */
    bool Initialize(HWND hWnd);

    /**
    * Renders the 3D scene
    * @param scene The elements making up the scene
    * @param timer The time passed since scene start
    */
    void Render();

    /**
    * @return the directX device
    */
    ID3D11Device* GetDevice() const;

private:

    /**
    * Initialises the DirectX debugging layer
    * @note only for _DEBUG
    */
    void InitialiseDebugging();

    HWND m_hwnd = nullptr;               
    std::unique_ptr<DirectxData> m_data;
};                     