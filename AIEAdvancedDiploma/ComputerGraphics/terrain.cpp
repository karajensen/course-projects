////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"

Terrain::Terrain(const std::string& name, int shaderID) :
    MeshData(name, shaderID)
{
}

float Terrain::Specularity() const
{
    return m_specularity;
}

float Terrain::Ambience() const
{
    return m_ambience;
}

bool Terrain::BackfaceCull() const
{
    return true;
}

float Terrain::Bump() const
{
    return m_bump;
}

float Terrain::Caustics() const
{
    return m_caustics;
}