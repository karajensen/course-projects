////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "directxcommon.h"
#include "boost/pending/disjoint_sets.hpp"

class SRM;

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
    void Render(ID3D11Texture2D* texture);

    /**
    * Sets the compute shader active
    */
    void SetActive();

    /**
    * @return the output buffer
    */
    ID3D11Texture2D* GetBuffer();

    /**
    * Sets the vectorization amount
    */
    void SetVectorization(float value);

    /**
    * @return the vectorization amount
    */
    bool RequiresVectorization() const;

private:

    /**
    * Compute shader initialisation
    */
    bool CreateShader(const char* file);
    bool CreateInputBuffer();
    bool CreateOutputBuffer();
    bool CreateConstantBuffer();

    /**
    * Union Find Data Structure to keep track of a set of elements
    * partitioned into a number of disjoint (nonoverlapping) subsets
    */
    typedef boost::disjoint_sets<int*, int*> DisjointSet;

    /**
    * 3 Component colour
    */
    struct Colour
    {
        float r, g, b;
    };

    /**
    * An edge: two indices and a difference value
    */
    struct Edge
    {
        int region1 = 0;
        int region2 = 0;
        int difference = 0;
    };

    /**
    * Constant data sent to the compute shader
    */
    struct Constants
    {
        int width = 0;
        int height = 0;
        int edges = 0;
    };

    /**
    * Main algorithm technique
    * Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java
    */
    bool MergePredicate(int reg1, int reg2);
    void MergeRegions(int C1, int C2, DisjointSet& set);
    void InitialiseSRM(ID3D11Texture2D* texture);
    void ExecuteSRM(DisjointSet& set);
    void OutputSRM(DisjointSet& set);

    int m_edges = 0;
    int m_width = 0;                  ///< Width of the image 
    int m_height = 0;                 ///< Height of the image 
    int m_size = 0;                   ///< width x height of the image 
    double m_complexity = 0.0;        ///< Complexity of regions generated
    int m_levels = 256;               ///< Number of levels in a color channel
    int m_levelsSqr = 0;              ///< Levels value squared
    double m_logdelta = 0.0;          ///< Constant for algorithm

    float m_vectorization = 0.0f;
    size_t m_srcBufferSize = 0;
    size_t m_srcBufferStride = 0;
    size_t m_destBufferSize = 0;
    size_t m_destBufferStride = 0;
    size_t m_constantBufferSize = 0;
    size_t m_constantBufferStride = 0;
    ID3D11DeviceContext* m_context = nullptr;
    ID3D11Device* m_device = nullptr;
    ID3D11ComputeShader* m_shader = nullptr;
    ID3D11Buffer* m_srcBuffer = nullptr;
    ID3D11ShaderResourceView* m_srcBufferView = nullptr;
    ID3D11Buffer* m_constantBuffer = nullptr;
    ID3D11ShaderResourceView* m_constantBufferView = nullptr;
    ID3D11Buffer* m_destBuffer = nullptr;
    ID3D11Buffer* m_destBufferSystem = nullptr;
    ID3D11UnorderedAccessView* m_destBufferView = nullptr;
    ID3D11Texture2D* m_destTexture = nullptr;
    ID3D11Texture2D* m_srcTexture = nullptr;

    std::vector<int> m_count;
    std::vector<Colour> m_average;
    std::vector<int> m_rank;
    std::vector<int> m_parent;
};