////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vectorization.h"
#include "SRM.h"
#include "directx/include/D3Dcompiler.h"
#include <sstream>

Vectorization::Vectorization()
{
}

Vectorization::~Vectorization()
{
    Release();
}

void Vectorization::Release()
{
    SafeRelease(&m_srcTexture);
    SafeRelease(&m_destTexture);
    SafeRelease(&m_srcBuffer);
    SafeRelease(&m_srcBufferView);
    SafeRelease(&m_valuesBuffer);
    SafeRelease(&m_valuesBufferView);
    SafeRelease(&m_destBuffer);
    SafeRelease(&m_destBufferSystem);
    SafeRelease(&m_destBufferView);
    SafeRelease(&m_shader);
}

void Vectorization::UpdateValuesBuffer()
{
    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    if (FAILED(m_context->Map(m_valuesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
    {
        MessageBox(0, "Failed to map values buffer", "ERROR", MB_OK);
        return;
    }

    *reinterpret_cast<ValuesData*>(mappedBuffer.pData) = m_valuesData;

    m_context->Unmap(m_valuesBuffer, 0);
}

void Vectorization::SetVectorization(float value)
{
    m_valuesData.vectorization = value;
    UpdateValuesBuffer();
}

float Vectorization::GetVectorization() const
{
    return m_valuesData.vectorization;
}

bool Vectorization::CreateShader(const char* file)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    LPCSTR profile = (m_device->GetFeatureLevel() >=
        D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

    ID3DBlob* errorBlob = nullptr;
    ID3DBlob* shaderBlob = nullptr;
    HRESULT hr = D3DX11CompileFromFile(
        file,
        0, 0, "CSMain",
        profile,
        flags,
        0, 0,
        &shaderBlob,
        &errorBlob, 0);

    if (FAILED(hr))
    {
        std::ostringstream stream;
        stream << "Failed to compile compute shader: ";

        if (errorBlob)
        {
            stream << (char*)errorBlob->GetBufferPointer();
            errorBlob->Release();
        }
        else
        {
            stream << "Unknown Error";
        }

        if (shaderBlob)
        {
            shaderBlob->Release();
        }

        MessageBox(0, stream.str().c_str(), "ERROR", MB_OK);
        return false;
    }

    if (errorBlob)
    {
        errorBlob->Release();
    }

    if (FAILED(m_device->CreateComputeShader(shaderBlob->GetBufferPointer(),
                                             shaderBlob->GetBufferSize(),
                                             nullptr, &m_shader)))
    {
        MessageBox(0, "Failed to create compute shader", "ERROR", MB_OK);
        shaderBlob->Release();
        return false;
    }

    shaderBlob->Release();

    SetDebugName(m_shader, "Compute Shader");

    m_context->CSSetShader(m_shader, NULL, 0);
    m_context->CSSetShaderResources(0, 1, &m_srcBufferView);
    m_context->CSSetShaderResources(1, 1, &m_valuesBufferView);
    m_context->CSSetUnorderedAccessViews(0, 1, &m_destBufferView, NULL);

    return true;
}

ID3D11Texture2D* Vectorization::GetBuffer()
{
    return m_destTexture;
}

void Vectorization::CopyToBuffer(ID3D11Texture2D* texture)
{
    m_requiresRender = true;

    if (m_valuesData.vectorization == 0.0f)
    {
        m_context->CopyResource(m_destTexture, texture);
    }
    else
    {
        m_context->CopyResource(m_srcTexture, texture);

        D3D11_MAPPED_SUBRESOURCE mappedTex;
        if (FAILED(m_context->Map(m_srcTexture, 0, D3D11_MAP_READ, 0, &mappedTex)))
        {
            MessageBox(0, "Failed to map input texture", "ERROR", MB_OK);
            return;
        }

        if (m_useComputeShader)
        {
            D3D11_MAPPED_SUBRESOURCE mappedBuffer;
            if (FAILED(m_context->Map(m_srcBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
            {
                MessageBox(0, "Failed to map input buffer", "ERROR", MB_OK);
                return;
            }

            memcpy(mappedBuffer.pData, mappedTex.pData, m_bufferSize);

            m_context->Unmap(m_srcBuffer, 0);
        }
        else
        {
            D3D11_MAPPED_SUBRESOURCE mappedOutput;
            if (FAILED(m_context->Map(m_destTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedOutput)))
            {
                MessageBox(0, "Failed to map output texture", "ERROR", MB_OK);
                return;
            }

            auto* output = reinterpret_cast<int*>(mappedOutput.pData);
            auto* input = reinterpret_cast<int*>(mappedTex.pData);
            m_srm->Execute(input, output, m_valuesData.vectorization, m_border);

            m_context->Unmap(m_destTexture, 0);
        }

        m_context->Unmap(m_srcTexture, 0);
    }
}

bool Vectorization::RequiresRendering() const
{
    return m_requiresRender;
}

void Vectorization::Render()
{
    if (!m_requiresRender)
    {
        return;
    }
    m_requiresRender = false;

    if (m_useComputeShader && m_valuesData.vectorization != 0.0f)
    {
        m_context->Dispatch(1, 1, 1);
        m_context->CopyResource(m_destBufferSystem, m_destBuffer);

        D3D11_MAPPED_SUBRESOURCE mappedBuffer;
        if (FAILED(m_context->Map(m_destBufferSystem, 0, D3D11_MAP_READ, 0, &mappedBuffer)))
        {
            MessageBox(0, "Failed to map output buffer", "ERROR", MB_OK);
            return;
        }

        D3D11_MAPPED_SUBRESOURCE mappedTex;
        if (FAILED(m_context->Map(m_destTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex)))
        {
            MessageBox(0, "Failed to map output texture", "ERROR", MB_OK);
            return;
        }

        memcpy(mappedTex.pData, mappedBuffer.pData, m_bufferSize);

        m_context->Unmap(m_destBufferSystem, 0);
        m_context->Unmap(m_destTexture, 0);
    }
}

void Vectorization::ToggleBorder()
{
    m_border = !m_border;
}

bool Vectorization::Initialise(ID3D11Device* device, 
                               ID3D11DeviceContext* context,
                               const char* file, 
                               const POINT& size)
{
    m_screen = size;
    m_device = device;
    m_context = context;
    m_bufferStride = sizeof(int);
    m_bufferSize = (m_bufferStride * size.x) * size.y;
    m_srm = std::make_unique<SRM>(size.x, size.y);

    return CreateInputBuffer() && CreateOutputBuffer() && CreateValuesBuffer() && CreateShader(file);
}

bool Vectorization::CreateInputBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descBuffer.ByteWidth = m_bufferSize;
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.StructureByteStride = m_bufferStride;
    descBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    descBuffer.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(m_device->CreateBuffer(&descBuffer, NULL, &m_srcBuffer)))
    {
        MessageBox(0, "Failed to create compute shader input buffer", "ERROR", MB_OK);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC descView;
    ZeroMemory(&descView, sizeof(descView));
    descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    descView.BufferEx.FirstElement = 0;
    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.BufferEx.NumElements = descBuffer.ByteWidth / descBuffer.StructureByteStride;

    if (FAILED(m_device->CreateShaderResourceView(m_srcBuffer, &descView, &m_srcBufferView)))
    {
        MessageBox(0, "Failed to create compute shader input view", "ERROR", MB_OK);
        return false;
    }

    D3D11_TEXTURE2D_DESC descTex;
    descTex.Width = m_screen.x;
    descTex.Height = m_screen.y;
    descTex.MipLevels = 1;
    descTex.ArraySize = 1;
    descTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.BindFlags = 0;
    descTex.Usage = D3D11_USAGE_STAGING;
    descTex.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    descTex.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&descTex, 0, &m_srcTexture)))
    {
        MessageBox(0, "Failed to create src texture", "ERROR", MB_OK);
        return false;
    }

    SetDebugName(m_srcBuffer, "Compute Shader Input Buffer");
    SetDebugName(m_srcBufferView, "Compute Shader Input View");
    SetDebugName(m_srcTexture, "Compute Shader Input Texture");

    return true;
}

bool Vectorization::CreateValuesBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descBuffer.ByteWidth = sizeof(ValuesData);
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.StructureByteStride = sizeof(ValuesData);
    descBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    descBuffer.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(m_device->CreateBuffer(&descBuffer, NULL, &m_valuesBuffer)))
    {
        MessageBox(0, "Failed to create compute shader values buffer", "ERROR", MB_OK);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC descView;
    ZeroMemory(&descView, sizeof(descView));
    descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    descView.BufferEx.FirstElement = 0;
    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.BufferEx.NumElements = descBuffer.ByteWidth / descBuffer.StructureByteStride;

    if (FAILED(m_device->CreateShaderResourceView(m_valuesBuffer, &descView, &m_valuesBufferView)))
    {
        MessageBox(0, "Failed to create compute shader values view", "ERROR", MB_OK);
        return false;
    }

    SetDebugName(m_srcBuffer, "Compute Shader Values Buffer");
    SetDebugName(m_srcBufferView, "Compute Shader Values View");

    return true;
}

bool Vectorization::CreateOutputBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    descBuffer.ByteWidth = m_bufferSize;
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.Usage = D3D11_USAGE_DEFAULT;
    descBuffer.StructureByteStride = m_bufferStride;

    if (FAILED(m_device->CreateBuffer(&descBuffer, NULL, &m_destBuffer)))
    {
        MessageBox(0, "Failed to create compute shader output buffer", "ERROR", MB_OK);
        return false;
    }

    descBuffer.Usage = D3D11_USAGE_STAGING;
    descBuffer.BindFlags = 0;
    descBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    if (FAILED(m_device->CreateBuffer(&descBuffer, NULL, &m_destBufferSystem)))
    {
        MessageBox(0, "Failed to create compute shader output system buffer", "ERROR", MB_OK);
        return false;
    }

    D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
    ZeroMemory(&descView, sizeof(descView));
    descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.Buffer.FirstElement = 0;
    descView.Buffer.Flags = 0;
    descView.Buffer.NumElements = descBuffer.ByteWidth / descBuffer.StructureByteStride;

    if (FAILED(m_device->CreateUnorderedAccessView(m_destBuffer, &descView, &m_destBufferView)))
    {
        MessageBox(0, "Failed to create compute shader output view", "ERROR", MB_OK);
        return false;
    }

    D3D11_TEXTURE2D_DESC descTex;
    descTex.Width = m_screen.x;
    descTex.Height = m_screen.y;
    descTex.MipLevels = 1;
    descTex.ArraySize = 1;
    descTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descTex.Usage = D3D11_USAGE_DYNAMIC;
    descTex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    descTex.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&descTex, 0, &m_destTexture)))
    {
        MessageBox(0, "Failed to create dest texture", "ERROR", MB_OK);
        return false;
    }

    SetDebugName(m_destBuffer, "Compute Shader Output Buffer");
    SetDebugName(m_destBufferView, "Compute Shader Output View");
    SetDebugName(m_destTexture, "Compute Shader Output Texture");

    return true;
}