////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtarget.h"
#include <assert.h>

D3D11_TEXTURE2D_DESC DxRenderTarget::sm_textureDesc;

DxRenderTarget::DxRenderTarget(const std::string& name, bool isBackBuffer) :
    m_isBackBuffer(isBackBuffer),
    m_name(name),
    m_texture(nullptr),
    m_view(nullptr),
    m_target(nullptr)
{
    
}

DxRenderTarget::~DxRenderTarget()
{
    Release();
}

void DxRenderTarget::Release()
{
    SafeRelease(&m_texture);
    SafeRelease(&m_target);
    SafeRelease(&m_view);
    SafeRelease(&m_depthBuffer);
}

bool DxRenderTarget::InitialiseDepthBuffer(ID3D11Device* device)
{
    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;

    ID3D11Texture2D* depthTexture;
    if(FAILED(device->CreateTexture2D(&textureDesc, 0, &depthTexture)))
    {
        MessageBox(0, "DirectX: Depth buffer texture creation failed", "ERROR", MB_OK);
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Format = textureDesc.Format;
    depthBufferDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthBufferDesc.Texture2D.MipSlice = 0;

    if(FAILED(device->CreateDepthStencilView(
        depthTexture, &depthBufferDesc, &m_depthBuffer)))
    {
        MessageBox(0, "DirectX: Depth buffer creation failed", "ERROR", MB_OK);
        return false;
    }

    SetDebugName(depthTexture, m_name + "DepthBufferTexture");
    SetDebugName(m_depthBuffer, m_name + "DepthBuffer");
    depthTexture->Release();

    return true;
}

bool DxRenderTarget::InitialiseBackBuffer(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(m_texture));
    if (FAILED(device->CreateRenderTargetView(m_texture, nullptr, &(m_target))))
    {
        MessageBox(0, "DirectX: Failed to create back buffer", "ERROR", MB_OK);
        return false;
    }

    m_texture->GetDesc(&sm_textureDesc);

    SetDebugName(m_texture, m_name + "_Texture");
    SetDebugName(m_target, m_name + "_RenderTarget");

    return true;
}

bool DxRenderTarget::InitialiseRenderTarget(ID3D11Device* device)
{
    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (FAILED(device->CreateTexture2D(&textureDesc, 0, &m_texture)))
    {
        MessageBox(0, ("DirectX: Failed to create texture for " + m_name).c_str(), "ERROR", MB_OK);
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
    ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
    renderTargetDesc.Format = textureDesc.Format;
    renderTargetDesc.Texture2D.MipSlice = 0;
    renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    if (FAILED(device->CreateRenderTargetView(m_texture, &renderTargetDesc, &m_target)))
    {
        MessageBox(0, ("DirectX: Failed to create render target for " + m_name).c_str(), "ERROR", MB_OK);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    ZeroMemory(&textureViewDesc, sizeof(textureViewDesc));
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.Texture2D.MipLevels = 1;
    textureViewDesc.Texture2D.MostDetailedMip = 0;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    if (FAILED(device->CreateShaderResourceView(m_texture, &textureViewDesc, &m_view)))
    {
        MessageBox(0, ("DirectX: Failed to create texture view for " + m_name).c_str(), "ERROR", MB_OK);
        return false;
    }

    SetDebugName(m_texture, m_name + "_Texture");
    SetDebugName(m_target, m_name + "_RenderTarget");
    SetDebugName(m_view, m_name + "_TextureView");

    return true;
}

bool DxRenderTarget::Initialise(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    assert(m_isBackBuffer);

    return InitialiseBackBuffer(device, swapchain);
}

bool DxRenderTarget::Initialise(ID3D11Device* device, ID3D11SamplerState* state)
{
    assert(!m_isBackBuffer);

    m_state = state;

    if (!InitialiseDepthBuffer(device))
    {
        MessageBox(0, "DirectX: Failed to create depth buffer", "ERROR", MB_OK);
        return false;
    }

    if (!InitialiseRenderTarget(device))
    {
        MessageBox(0, "DirectX: Failed to create render target", "ERROR", MB_OK);
        return false;
    }

    return true;
}

void DxRenderTarget::SetActive(ID3D11DeviceContext* context)
{
    if (m_isBackBuffer)
    {
        context->OMSetRenderTargets(1, &m_target, nullptr);
    }
    else
    {
        context->OMSetRenderTargets(1, &m_target, m_depthBuffer);
    }

    ClearTarget(context);
}

void DxRenderTarget::ClearTarget(ID3D11DeviceContext* context)
{
    if (!m_isBackBuffer)
    {
        context->ClearDepthStencilView(m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    context->ClearRenderTargetView(m_target, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
}

ID3D11ShaderResourceView* const* DxRenderTarget::GetView() const
{
    return &m_view;
}

ID3D11Texture2D* DxRenderTarget::GetTexture() const
{
    return m_texture;
}

ID3D11SamplerState* const* DxRenderTarget::State() const
{
    return &m_state;
}