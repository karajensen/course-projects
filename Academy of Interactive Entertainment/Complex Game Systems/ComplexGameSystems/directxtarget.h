////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include <vector>

class DxRenderTarget
{
public:

    /**
    * Constructor for the back buffer target
    * @param name Name of the render target
    */
    DxRenderTarget(const std::string& name, bool isBackBuffer);

    /**
    * Destructor
    */
    ~DxRenderTarget();

    /**
    * Releases the render target
    */
    void Release();

    /**
    * Initialises the back buffer
    * @param device The DirectX device interface
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, IDXGISwapChain* swapchain);

    /**
    * Initialises a render target
    * @param device The DirectX device interface
    * @param state The sampler state to render this target
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, ID3D11SamplerState* state);

    /**
    * Sets the render target as activated and clears it
    * @param context Direct3D device context
    */
    void SetActive(ID3D11DeviceContext* context);

    /**
    * Gets the render target texture
    */
    ID3D11ShaderResourceView* const* GetView() const;

    /**
    * Gets the render target texture
    */
    ID3D11Texture2D* GetTexture() const;

    /**
    * @return the sampler state of this render target
    */
    ID3D11SamplerState* const* State() const;

private:

    /**
    * Clears the render target
    * @param context Direct3D device context
    */
    void ClearTarget(ID3D11DeviceContext* context);

    /**
    * Initialises a render target
    * @param device The DirectX device interface
    * @return whether initialise succeeded or not
    */
    bool InitialiseRenderTarget(ID3D11Device* device);

    /**
    * Initialises the depth buffer
    * @param device The DirectX device interface
    * @return whether initialise succeeded or not
    */
    bool InitialiseDepthBuffer(ID3D11Device* device);

    /**
    * Initialises the back buffer
    * @param device The DirectX device interface
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialise succeeded or not
    */
    bool InitialiseBackBuffer(ID3D11Device* device, IDXGISwapChain* swapchain);

    const bool m_isBackBuffer = false;                  ///< Whether this render target is the back buffer
    const std::string m_name;                           ///< Name of the render target
    ID3D11Texture2D* m_texture;                         ///< Textures to render to
    ID3D11RenderTargetView* m_target;                   ////< Render target buffers
    ID3D11ShaderResourceView* m_view;                   ///< Shader views for the textures
    ID3D11DepthStencilView* m_depthBuffer = nullptr;    ///< Depth buffer for the render target
    static D3D11_TEXTURE2D_DESC sm_textureDesc;         ///< Base description of the render target textures
    ID3D11SamplerState* m_state = nullptr;              ///< The sampler state for rendering the target
};