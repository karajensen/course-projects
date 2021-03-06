////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include <Windows.h>
#include <memory>

struct DirectxData;

/**
* DirectX Graphics engine
*/
class DirectxEngine
{
public:

    /**
    * Constructor
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
    bool Initialize(HWND hWnd, const POINT& size);

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

    /**
    * @return the directX context
    */
    ID3D11DeviceContext* GetContext() const;

    /**
    * @return the back buffer
    */
    ID3D11Texture2D* GetBackBuffer() const;

    /**
    * Saves the current frame
    */
    void Save();

private:

    /**
    * Initialises the DirectX debugging layer
    * @note only for _DEBUG
    */
    void InitialiseDebugging();

    HWND m_hwnd = nullptr;               
    std::unique_ptr<DirectxData> m_data;
    POINT m_screen;
};                     