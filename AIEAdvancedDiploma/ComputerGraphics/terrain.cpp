////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"

Terrain::Terrain(const std::string& name, int shaderID) :
    Grid(name, shaderID)
{
}

bool Terrain::Initialise(Type type,
                         const glm::vec3& position,
                         float spacing,
                         int size)
{
    m_type = type;

    // Only odd numbered grids are supported
    size = (size % 2) ? size : size + 1;

    CreateGrid(position, spacing, size, size, true);

    GenerateTerrain();

    RecalculateNormals();

    return MeshData::Initialise();
}

void Terrain::GenerateTerrain()
{
    switch (m_type)
    {
    case DIAMOND_SQUARE:
        GenerateDiamondSquareTerrain();
        break;
    default:
        LogError("Unknown terrain type");
    }
}

bool Terrain::Reload()
{
    ResetGrid();

    GenerateTerrain();

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

void Terrain::GenerateDiamondSquareTerrain()
{
}