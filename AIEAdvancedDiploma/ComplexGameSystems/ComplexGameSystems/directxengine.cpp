////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxtarget.h"
#include <array>
#include <fstream>
#include <sstream>

/**
* Internal data for the directx rendering engine
*/
struct DirectxData
{
    /**
    * Constructor
    */
    DirectxData();

    /**
    * Destructor
    */
    ~DirectxData();

    IDXGISwapChain* swapchain = nullptr;   
    ID3D11Device* device = nullptr;        
    ID3D11DeviceContext* context = nullptr;
    ID3D11Debug* debug = nullptr;          
    DxRenderTarget backBuffer;
};

DirectxData::DirectxData() :
    backBuffer("BackBuffer", true)
{
}

DirectxData::~DirectxData()
{
    backBuffer.Release();
    SafeRelease(&swapchain);
    SafeRelease(&context);
    SafeRelease(&device);

    if(debug)
    {
        debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        debug->Release();
        debug = nullptr;

        std::string seperator(100, '=');
        OutputDebugString((seperator + "\n").c_str());
    }
}

DirectxEngine::DirectxEngine() :
    m_data(new DirectxData())
{
}

DirectxEngine::~DirectxEngine()
{
}

bool DirectxEngine::Initialize(HWND hWnd, const POINT& size)
{
    m_hwnd = hWnd;
    m_screen = size;

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.OutputWindow = m_hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.BufferDesc.Width = size.x;
    scd.BufferDesc.Height = size.y;

#ifdef _DEBUG
    unsigned int deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
    unsigned int deviceFlags = 0;
#endif

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &scd,
        &m_data->swapchain, &m_data->device, nullptr, &m_data->context)))
    {
        MessageBox(0, "DirectX: Device creation failed", "ERROR", MB_OK);
        return false;
    }

    InitialiseDebugging();

    // Create the render targets. Back buffer must be initialised first.
    if (!m_data->backBuffer.Initialise(m_data->device, m_data->swapchain))
    {
        return false;
    }

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(size.x);
    viewport.Height = static_cast<FLOAT>(size.y);
    viewport.MinDepth = 0.0;
    viewport.MaxDepth = 1.0;
    m_data->context->RSSetViewports(1, &viewport);

    SetDebugName(m_data->device, "Device");
    SetDebugName(m_data->context, "Context");
    SetDebugName(m_data->swapchain, "SwapChain");

    m_data->backBuffer.SetActive(m_data->context);
    return true;
}

ID3D11Texture2D* DirectxEngine::GetBackBuffer() const
{
    return m_data->backBuffer.GetTexture();
}

ID3D11DeviceContext* DirectxEngine::GetContext() const
{
    return m_data->context;
}

void DirectxEngine::InitialiseDebugging()
{
    #ifdef _DEBUG
    if(SUCCEEDED(m_data->device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_data->debug)))
    {
        ID3D11InfoQueue* infoQueue = nullptr;
        if(SUCCEEDED(m_data->debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)))
        {
            D3D11_MESSAGE_ID knownMessages[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
            };
 
            D3D11_INFO_QUEUE_FILTER filter;
            memset( &filter, 0, sizeof(filter) );
            filter.DenyList.NumIDs = _countof(knownMessages);
            filter.DenyList.pIDList = knownMessages;

            infoQueue->AddStorageFilterEntries(&filter);
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            infoQueue->Release();
        }
    }
    #endif
}

void DirectxEngine::Render()
{
    m_data->swapchain->Present(0, 0);
}

ID3D11Device* DirectxEngine::GetDevice() const
{
    return m_data->device;
}

void DirectxEngine::Save()
{
    D3DX11SaveTextureToFile(m_data->context, 
        m_data->backBuffer.GetTexture(), D3DX11_IFF_PNG, "frame.png");

    MessageBox(0, "Saved current frame", "INFO", MB_OK);
}
