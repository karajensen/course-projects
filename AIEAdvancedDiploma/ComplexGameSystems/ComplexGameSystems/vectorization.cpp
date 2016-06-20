////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vectorization.h"
#include "directx/include/D3Dcompiler.h"
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>

namespace
{
    int Red(int colour)
    {
        return colour & 0xFF;
    }

    int Green(int colour)
    {
        return (colour & 0xFF00) >> 8;
    }

    int Blue(int colour)
    {
        return (colour & 0xFF0000) >> 16;
    }

    int ToColour(int r, int g, int b)
    {
        return 0xff000000 | b << 16 | g << 8 | r;
    }
}

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
    SafeRelease(&m_constantBuffer);
    SafeRelease(&m_constantBufferView);
    SafeRelease(&m_destBuffer);
    SafeRelease(&m_destBufferSystem);
    SafeRelease(&m_destBufferView);
    SafeRelease(&m_shader);
}

void Vectorization::SetVectorization(float value)
{
    m_vectorization = value;
}

bool Vectorization::RequiresVectorization() const
{
    return m_vectorization != 0.0f;
}

ID3D11Texture2D* Vectorization::GetBuffer()
{
    return m_destTexture;
}

void Vectorization::InitialiseSRM(ID3D11Texture2D* texture)
{
    // Copy the texture to the input buffer
    m_context->CopyResource(m_srcTexture, texture);

    D3D11_MAPPED_SUBRESOURCE mappedTex;
    if (FAILED(m_context->Map(m_srcTexture, 0, D3D11_MAP_READ, 0, &mappedTex)))
    {
        MessageBox(0, "Failed to map input texture", "ERROR", MB_OK);
        return;
    }

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    if (FAILED(m_context->Map(m_srcBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
    {
        MessageBox(0, "Failed to map input buffer", "ERROR", MB_OK);
        return;
    }

    memcpy(mappedBuffer.pData, mappedTex.pData, m_srcBufferSize);
    m_context->Unmap(m_srcBuffer, 0);

    // Dispatch the call to the compute shader
    m_context->Dispatch(32, 32, 1);

    // Initialise all containers
    std::iota(m_parent.begin(), m_parent.end(), 0);
    m_count.assign(m_size, 1);
    m_rank.assign(m_size, 0);

    // complexity is inverse to vectorization range [0,1]
    m_complexity = 30.0 + ((1.0 - m_vectorization) * 300.0);

    auto* input = reinterpret_cast<int*>(mappedTex.pData);

    for (int i = 0; i < m_size; ++i)
    {
        m_average[i].r = (float)Red(input[i]);
        m_average[i].g = (float)Green(input[i]);
        m_average[i].b = (float)Blue(input[i]);
    }

    m_context->Unmap(m_srcTexture, 0);
}

void Vectorization::SetActive()
{
    m_context->CSSetShader(m_shader, 0, 0);
    m_context->CSSetShaderResources(0, 1, &m_srcBufferView);
    m_context->CSSetShaderResources(1, 1, &m_constantBufferView);
    m_context->CSSetUnorderedAccessViews(0, 1, &m_destBufferView, 0);
}

void Vectorization::ExecuteSRM(DisjointSet& set)
{
    m_context->CopyResource(m_destBufferSystem, m_destBuffer);

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    if (FAILED(m_context->Map(m_destBufferSystem, 0, D3D11_MAP_READ, 0, &mappedBuffer)))
    {
        MessageBox(0, "Failed to map output buffer", "ERROR", MB_OK);
        return;
    }

    auto* output = reinterpret_cast<Edge*>(mappedBuffer.pData);

    std::sort(output, output + m_edges,
        [](const Edge& e1, const Edge& e2)
    {
        return e1.difference < e2.difference;
    });

    for (int i = 0; i < m_edges; i++)
    {
        const int r1 = output[i].region1;
        const int r2 = output[i].region2;

        const int C1 = set.find_set(r1);
        const int C2 = set.find_set(r2);

        if (C1 != C2 && MergePredicate(C1, C2))
        {
            MergeRegions(C1, C2, set);
        }
    }

    m_context->Unmap(m_destBufferSystem, 0);
}

void Vectorization::MergeRegions(int C1, int C2, DisjointSet& set)
{
    set.union_set(C1, C2);

    const int region = m_rank[C1] > m_rank[C2] ? C1 : C2;
    const int nreg = m_count[C1] + m_count[C2];
    const float fnreg = 1.0f / (float)nreg;
    const int c1 = m_count[C1];
    const int c2 = m_count[C2];
    const auto& a1 = m_average[C1];
    const auto& a2 = m_average[C2];

    m_average[region].r = (c1 * a1.r + c2 * a2.r) * fnreg;
    m_average[region].g = (c1 * a1.g + c2 * a2.g) * fnreg;
    m_average[region].b = (c1 * a1.b + c2 * a2.b) * fnreg;

    m_count[region] = nreg;
}

bool Vectorization::MergePredicate(int reg1, int reg2)
{
    const double c1 = (double)m_count[reg1];
    const double c2 = (double)m_count[reg2];

    const double logreg1 = min(double(m_levels), c1 * log(1.0 + c1));
    const double logreg2 = min(double(m_levels), c2 * log(1.0 + c2));

    const double dev1 = (m_levelsSqr / (2.0 * m_complexity * c1)) * (logreg1 + m_logdelta);
    const double dev2 = (m_levelsSqr / (2.0 * m_complexity * c2)) * (logreg2 + m_logdelta);
    const double dev = dev1 + dev2;

    return pow(m_average[reg1].r - m_average[reg2].r, 2) < dev &&
           pow(m_average[reg1].g - m_average[reg2].g, 2) < dev &&
           pow(m_average[reg1].b - m_average[reg2].b, 2) < dev;
}

void Vectorization::OutputSRM(DisjointSet& set)
{
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    if (FAILED(m_context->Map(m_destTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex)))
    {
        MessageBox(0, "Failed to map output texture", "ERROR", MB_OK);
        return;
    }

    auto* output = reinterpret_cast<int*>(mappedTex.pData);

    for (int i = 0; i < m_size; ++i)
    {
        const auto& c = m_average[set.find_set(i)];
        output[i] = ToColour((int)c.r, (int)c.g, (int)c.b);
    }

    m_context->Unmap(m_destTexture, 0);
}

void Vectorization::Render(ID3D11Texture2D* texture)
{
    InitialiseSRM(texture);

    DisjointSet set(&m_rank[0], &m_parent[0]);

    ExecuteSRM(set);

    OutputSRM(set);
}

bool Vectorization::Initialise(ID3D11Device* device, 
                               ID3D11DeviceContext* context,
                               const char* file, 
                               const POINT& size)
{
    const int w = size.x;
    const int h = size.y;
    m_width = w;
    m_height = h;
    m_size = w * h;
    m_logdelta = 2.0 * log(6.0 * m_size);
    m_levelsSqr = m_levels * m_levels;
    m_edges = 2 * (w - 1) * (h - 1) + (h - 1) + (w - 1);

    m_average.resize(m_size);
    m_count.resize(m_size);
    m_parent.resize(m_size);
    m_rank.resize(m_size);

    m_device = device;
    m_context = context;
    m_srcBufferStride = sizeof(int);
    m_srcBufferSize = (m_srcBufferStride * w) * h;
    m_destBufferStride = sizeof(Edge);
    m_destBufferSize = m_destBufferStride * m_edges;
    m_constantBufferStride = sizeof(Constants);
    m_constantBufferSize = m_constantBufferStride;

    if (!CreateInputBuffer() ||
        !CreateOutputBuffer() ||
        !CreateConstantBuffer() ||
        !CreateShader(file))
    {
        return false;
    }

    SetVectorization(0.0f);
    return true;
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

    if (FAILED(m_device->CreateComputeShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr, &m_shader)))
    {
        MessageBox(0, "Failed to create compute shader", "ERROR", MB_OK);
        shaderBlob->Release();
        return false;
    }

    shaderBlob->Release();

    SetDebugName(m_shader, "Compute Shader");

    m_context->CSSetShader(m_shader, 0, 0);
    m_context->CSSetShaderResources(0, 1, &m_srcBufferView);
    m_context->CSSetShaderResources(1, 1, &m_constantBufferView);
    m_context->CSSetUnorderedAccessViews(0, 1, &m_destBufferView, 0);

    return true;
}

bool Vectorization::CreateInputBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descBuffer.ByteWidth = m_srcBufferSize;
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.StructureByteStride = m_srcBufferStride;
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
    descTex.Width = m_width;
    descTex.Height = m_height;
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

bool Vectorization::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descBuffer.ByteWidth = m_constantBufferSize;
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.StructureByteStride = m_constantBufferStride;
    descBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    descBuffer.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(m_device->CreateBuffer(&descBuffer, NULL, &m_constantBuffer)))
    {
        MessageBox(0, "Failed to create compute shader constant buffer", "ERROR", MB_OK);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC descView;
    ZeroMemory(&descView, sizeof(descView));
    descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    descView.BufferEx.FirstElement = 0;
    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.BufferEx.NumElements = descBuffer.ByteWidth / descBuffer.StructureByteStride;

    if (FAILED(m_device->CreateShaderResourceView(m_constantBuffer, &descView, &m_constantBufferView)))
    {
        MessageBox(0, "Failed to create compute shader constant view", "ERROR", MB_OK);
        return false;
    }

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    if (FAILED(m_context->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer)))
    {
        MessageBox(0, "Failed to map constant buffer", "ERROR", MB_OK);
        return false;
    }

    Constants constants;
    constants.height = m_height;
    constants.width = m_width;
    constants.edges = m_edges;

    memcpy(mappedBuffer.pData, &constants, m_constantBufferSize);

    m_context->Unmap(m_constantBuffer, 0);

    SetDebugName(m_constantBuffer, "Compute Shader Constant Buffer");
    SetDebugName(m_constantBufferView, "Compute Shader Constant View");

    return true;
}

bool Vectorization::CreateOutputBuffer()
{
    D3D11_BUFFER_DESC descBuffer;
    ZeroMemory(&descBuffer, sizeof(descBuffer));
    descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    descBuffer.ByteWidth = m_destBufferSize;
    descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.Usage = D3D11_USAGE_DEFAULT;
    descBuffer.StructureByteStride = m_destBufferStride;

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
    descTex.Width = m_width;
    descTex.Height = m_height;
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