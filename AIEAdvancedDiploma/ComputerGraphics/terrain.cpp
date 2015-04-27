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

    tweaker.AddFltEntry("Height", [this](){ return m_height; }, [this](const float value)
    { 
        m_height = value;
        for (auto& instance : Instances())
        {
            instance.position.y = m_height;
            instance.requiresUpdate = true;
        }
    }, 0.1f);

    tweaker.AddFltEntry("Min Height Offset", 
        [this](){ return m_minHeight; },
        [this](const float value){ m_minHeight = value; Reload(); }, 0.1f);

    tweaker.AddFltEntry("Max Height Offset", 
        [this](){ return m_maxHeight; },
        [this](const float value){ m_maxHeight = value; Reload(); }, 0.1f);

    tweaker.AddFltEntry("Texture Scale", 
        [this](){ return GetUVStretch().x; }, 
        [this](const float value)
    { 
        SetUVStretch(glm::vec2(value, value));
        Reload();
    }, 1.0f);

    if (UsesBumpMapping())
    {
        tweaker.AddFltEntry("Bump Amount", &m_bump, 0.1f);
    }

    if (UsesCaustics())
    {
        tweaker.AddFltEntry("Caustics Amount", &m_causticsAmount, 0.1f);
        tweaker.AddFltEntry("Caustics Scale", &m_causticsScale, 0.1f);
    }

    if (UsesSpecular())
    {
        tweaker.AddFltEntry("Specularity", &m_specularity, 0.1f);
    }

    tweaker.AddFltEntry("Specular", &m_specular, 0.1f);
    tweaker.AddFltEntry("Ambience", &m_ambience, 0.1f);
    tweaker.AddFltEntry("Diffuse", &m_diffuse, 0.1f);

    tweaker.AddButton("Reload", [this](){ Reload(); });
}

bool Terrain::Initialise(float uvStretch,
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

    if (CreateGrid(glm::vec2(uvStretch, uvStretch), 
        spacing, size, size, hasNormals, hasTangents))
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

void Terrain::GenerateTerrain()
{
    assert(Rows() == Columns());

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(m_pixels.size())));
    const double stepIncrease = static_cast<double>(mapSize / gridSize);
    double step = 0.0;

    for (int c = 0; c < gridSize; ++c)
    {
        for (int r = 0; r < gridSize; ++r)
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
    m_instances[index].requiresUpdate = true;
}

void Terrain::AddInstance(const glm::vec2& position)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), position);
}