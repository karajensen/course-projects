////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include <vector>

/**
* References:
* http://www.codinglabs.net/tutorial_compute_shaders_filters.aspx
* https://msdn.microsoft.com/en-us/library/windows/desktop/ff476330%28v=vs.85%29.aspx
* http://stackoverflow.com/questions/32049639/directx-11-compute-shader-writing-to-an-output-resource
*/
class Vectorization
{
public:

    /**
    * Constructor
    */
    Vectorization();

    /**
    * Destructor
    */
    ~Vectorization();

    /**
    * Releases the shader
    */
    void Release();

    /**
    * Sets up the compute shader
    * @return whether initialization succeeded
    */
    bool Initialise(ID3D11Device* device, 
                    ID3D11DeviceContext* context,
                    const char* file, 
                    const POINT& size);

    /**
    * Renders the compute shader
    */
    void Render();

    /**
    * Copies the texture to the input buffer
    */
    void CopyToBuffer(ID3D11Texture2D* texture);

    /**
    * @return the output buffer
    */
    ID3D11Texture2D* GetBuffer();

    /**
    * @return if the compute shader requires rendering
    */
    bool RequiresRendering() const;

    /**
    * Sets the vectorization amount
    */
    void SetVectorization(float value);

    /**
    * @return the vectorization amount
    */
    float GetVectorization() const;

private:

    bool CreateShader(const char* file);
    bool CreateInputBuffer();
    bool CreateOutputBuffer();
    bool CreateValuesBuffer();
    void UpdateValuesBuffer();

    struct ValuesData
    {
        float vectorization = 0.0f;
    };

    ValuesData m_valuesData;
    bool m_requiresRender = false;
    POINT m_screen;
    size_t m_bufferSize = 0;
    size_t m_bufferStride = 0;
    ID3D11DeviceContext* m_context = nullptr;
    ID3D11Device* m_device = nullptr;
    ID3D11ComputeShader* m_shader = nullptr;
    ID3D11Buffer* m_srcBuffer = nullptr;
    ID3D11ShaderResourceView* m_srcBufferView = nullptr;
    ID3D11Buffer* m_valuesBuffer = nullptr;
    ID3D11ShaderResourceView* m_valuesBufferView = nullptr;
    ID3D11Buffer* m_destBuffer = nullptr;
    ID3D11Buffer* m_destBufferSystem = nullptr;
    ID3D11UnorderedAccessView* m_destBufferView = nullptr;
    ID3D11Texture2D* m_destTexture = nullptr;
    ID3D11Texture2D* m_srcTexture = nullptr;
};