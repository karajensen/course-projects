////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"
#include "tweaker.h"

Terrain::Terrain(const std::string& name, 
                 const std::string& shaderName,
                 int shaderID,
                 const std::vector<unsigned int>& pixels) :

    Grid(name, shaderName, shaderID),
    m_pixels(pixels)
{
}

void Terrain::AddToTweaker(Tweaker& tweaker)
{
    Grid::AddToTweaker(tweaker);

    tweaker.AddEntry("Height", [this](){ return m_height; }, [this](const float value)
    { 
        m_height = value;
        for (auto& instance : Instances())
        {
            instance.position.y = m_height;
        }
    });

    tweaker.AddEntry("Min Height Offset", 
        [this](){ return m_minHeight; },
        [this](const float value){ m_minHeight = value; Reload(); });

    tweaker.AddEntry("Max Height Offset", 
        [this](){ return m_maxHeight; },
        [this](const float value){ m_maxHeight = value; Reload(); });

    tweaker.AddEntry("Texture Scale U", 
        [this](){ return GetUVStretch().x; }, 
        [this](const float value)
    { 
        SetUVStretch(glm::vec2(value, GetUVStretch().y));
        Reload(); 
    });

    tweaker.AddEntry("Texture Scale V", 
        [this](){ return GetUVStretch().y; }, 
        [this](const float value)
    { 
        SetUVStretch(glm::vec2(GetUVStretch().x, value));
        Reload(); 
    });

    tweaker.AddEntry("Ambience", &m_ambience, TW_TYPE_FLOAT, 0.1f);

    if (UsesBumpMapping())
    {
        tweaker.AddEntry("Bump Amount", &m_bump, TW_TYPE_FLOAT, 0.1f);
    }

    if (UsesCaustics())
    {
        tweaker.AddEntry("Caustics Amount", &m_causticsAmount, TW_TYPE_FLOAT, 0.1f);
        tweaker.AddEntry("Caustics Scale", &m_causticsScale, TW_TYPE_FLOAT, 0.1f);
    }

    if (UsesSpecular())
    {
        tweaker.AddEntry("Specularity", &m_specularity, TW_TYPE_FLOAT, 0.1f);
    }

    tweaker.AddButton("Reload", [this](){ Reload(); });
}

void Terrain::Specularity(float value)
{
    m_specularity = value;
}

void Terrain::Bump(float value)
{
    m_bump = value;
}

void Terrain::Ambience(float value)
{
    m_ambience = value;
}

void Terrain::CausticsAmount(float value)
{
    m_causticsAmount = value;
}

void Terrain::CausticsScale(float value)
{
    m_causticsScale = value;
}

bool Terrain::Initialise(const glm::vec2& uvStretch,
                         float minHeight,
                         float maxHeight,
                         float height,
                         float spacing,
                         int size,
                         bool hasNormals,
                         bool hasTangents)
{
    m_height = height;
    m_minHeight = minHeight;
    m_maxHeight = maxHeight;

    if (CreateGrid(uvStretch, spacing, size, size, hasNormals, hasTangents))
    {
        GenerateTerrain();
        RecalculateNormals();
        return MeshData::Initialise();
    }

    return false;
}

void Terrain::Reload()
{
    ResetGrid();
    GenerateTerrain();
    RecalculateNormals();
    if (!MeshData::Reload())
    {
        LogError("Terrain: Reload failed for " + Name());
    }
}

float Terrain::Specularity() const
{
    return m_specularity;
}

float Terrain::Ambience() const
{
    return m_ambience;
}

float Terrain::Bump() const
{
    return m_bump;
}

float Terrain::CausticsAmount() const
{
    return m_causticsAmount;
}

float Terrain::CausticsScale() const
{
    return m_causticsScale;
}

void Terrain::GenerateTerrain()
{
    assert(Rows() == Columns());

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(m_pixels.size())));
    const double stepIncrease = static_cast<double>(mapSize / gridSize);
    double step = 0.0;

    for (int r = 0; r < gridSize; ++r)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            const int index = static_cast<int>(std::round(step));
            const float colour = Clamp((m_pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
            const float height = ConvertRange(colour, 0.0f, 1.0f, m_minHeight, m_maxHeight);
            SetHeight(r, c, height);
            step += stepIncrease;
        }
    }

    // Ensure sides are matching for tiling
    for (int c = 0; c < gridSize; ++c)
    {
        SetHeight(0, c, GetHeight(gridSize-1, c));
    }
    for (int r = 0; r < gridSize; ++r)
    {
        SetHeight(r, 0, GetHeight(r, gridSize-1));
    }
}

void Terrain::SetInstance(int index, const glm::vec2& position)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
}

void Terrain::AddInstance(const glm::vec2& position)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), position);
}