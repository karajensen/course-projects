////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"

Terrain::Terrain(const std::string& name, int shaderID) :
    Grid(name, shaderID)
{
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

void Terrain::Caustics(float value)
{
    m_caustics = value;
}

bool Terrain::Initialise(const std::vector<unsigned int>& pixels,
                         const glm::vec3& position,
                         const glm::vec2& uvStretch,
                         float minHeight,
                         float maxHeight,
                         float spacing,
                         int size,
                         bool hasNormals,
                         bool hasTangents)
{
    m_minHeight = minHeight;
    m_maxHeight = maxHeight;

    if (CreateGrid(position, uvStretch, spacing, size, size, hasNormals, hasTangents))
    {
        GenerateTerrain(pixels);
        RecalculateNormals();
        return MeshData::Initialise();
    }

    return false;
}

bool Terrain::Reload(const std::vector<unsigned int>& pixels)
{
    ResetGrid();

    GenerateTerrain(pixels);

    return MeshData::Reload();
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

float Terrain::Caustics() const
{
    return m_caustics;
}

void Terrain::GenerateTerrain(const std::vector<unsigned int>& pixels)
{
    assert(Rows() == Columns());

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(pixels.size())));
    const double stepIncrease = static_cast<double>(mapSize / gridSize);
    double step = 0.0;

    for (int r = 0; r < gridSize; ++r)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            const int index = static_cast<int>(std::round(step));
            const float colour = Clamp((pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
            const float height = ConvertRange(colour, 0.0f, 1.0f, m_minHeight, m_maxHeight);
            SetHeight(r, c, height);
            step += stepIncrease;
        }
    }
}