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

    tweaker.AddFltEntry("Overall Specular", &m_specular, 0.1f);
    tweaker.AddFltEntry("Ambience", &m_ambience, 0.1f);
    tweaker.AddFltEntry("Minimum Diffuse", &m_diffuse, 0.1f);

    tweaker.AddButton("Reload", [this](){ Reload(); });
}

bool Terrain::Initialise(float uvTextureStretch,
                         float minHeight,
                         float maxHeight,
                         float height,
                         float spacing,
                         int size,
                         bool hasNormals,
                         bool hasTangents,
                         bool requiresTiling)
{
    m_height = height;
    m_minHeight = minHeight;
    m_maxHeight = maxHeight;
    m_requiresTiling = requiresTiling;

    if (CreateGrid(glm::vec2(uvTextureStretch, uvTextureStretch),
        spacing, size, size, hasNormals, hasTangents))
    {
        GenerateTerrain();
        RecalculateNormals();
        LogInfo("Terrain: Generated " + Name());

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
    const double stepIncrease = (mapSize / static_cast<double>(gridSize));
    double mapC = 0.0, mapR = 0.0;

    for (int r = 0; r < gridSize; ++r, mapR += stepIncrease)
    {
        for (int c = 0; c < gridSize; ++c, mapC += stepIncrease)
        {
            const int index = int(mapR) * mapSize + int(mapC);
            const float colour = Clamp((m_pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
            const float height = ConvertRange(colour, 0.0f, 1.0f, m_minHeight, m_maxHeight);
            SetHeight(r, c, height);
        }
        mapC = 0.0;
    }

    if (m_requiresTiling)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            SetHeight(0, c, GetHeight(gridSize-1, c));
        }
        for (int r = 0; r < gridSize; ++r)
        {
            SetHeight(r, 0, GetHeight(r, gridSize-1));
        }
    }
}

void Terrain::AddInstances(int amount)
{
    MeshData::AddInstances(amount);
    for (int i = 0; i < amount; ++i)
    {
        m_maxBounds.emplace_back();
        m_minBounds.emplace_back();
    }
}

void Terrain::CalculateBounds(int instance)
{
    const auto& instanceMesh = GetInstance(instance);
    const float size = Size();
    const float halfWidth = size * instanceMesh.scale.x  * 0.5f;
    const float halfLength = size * instanceMesh.scale.z  * 0.5f;
    m_minBounds[instance].x = instanceMesh.position.x - halfWidth;
    m_minBounds[instance].y = instanceMesh.position.z - halfLength;
    m_maxBounds[instance].x = instanceMesh.position.x + halfWidth;
    m_maxBounds[instance].y = instanceMesh.position.z + halfLength;
}

void Terrain::SetInstance(int index, 
                          const glm::vec3& position,
                          const glm::vec3& rotation,
                          const glm::vec3& scale)
{
    m_height = position.y;
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale = scale;
    m_instances[index].requiresUpdate = true;
    CalculateBounds(index);
}

void Terrain::SetInstance(int index, const glm::vec2& position)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
    m_instances[index].requiresUpdate = true;
    CalculateBounds(index);
}

void Terrain::AddInstance(const glm::vec2& position)
{
    Terrain::AddInstances(1);
    SetInstance(static_cast<int>(m_instances.size()-1), position);
}

glm::vec3 Terrain::GetAbsolutePosition(int instance, float x, float z)
{
    const auto& world = GetWorldInstance(instance);
    const float maxIndex = Rows() - 1.0f;
    const glm::vec2& minBounds = GetMinBounds(instance);
    const glm::vec2& maxBounds = GetMaxBounds(instance);

    const int row = static_cast<int>(std::round(Clamp(ConvertRange(
        x, minBounds.x, maxBounds.x, 0.0f, maxIndex), 0.0f, maxIndex)));

    const int column = static_cast<int>(std::round(Clamp(ConvertRange(
        z, minBounds.y, maxBounds.y, 0.0f, maxIndex), 0.0f, maxIndex)));

    const glm::vec4 position(GetPosition(row, column), 1.0f);
    return glm::vec3(world * position);
}

const glm::vec2& Terrain::GetMinBounds(int instance) const
{
    return m_minBounds[instance];
}

const glm::vec2& Terrain::GetMaxBounds(int instance) const
{
    return m_maxBounds[instance];
}