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

bool Terrain::Initialise(Type type,
                         const glm::vec3& position,
                         const glm::vec2& uvStretch,
                         float spacing,
                         int size,
                         bool hasNormals,
                         bool hasTangents)
{
    m_type = type;

    if (CreateGrid(position, uvStretch, spacing, size, size, hasNormals, hasTangents))
    {
        GenerateTerrain();
        RecalculateNormals();
        return MeshData::Initialise();
    }

    return false;
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