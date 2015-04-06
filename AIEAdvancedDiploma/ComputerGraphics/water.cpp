////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "water.h"

Water::Water(const std::string& name, int shaderID) :
    Grid(name, shaderID),
    m_speed(1.0f),
    m_bump(1.0f),
    m_bumpVelocity(0.0f, 0.0f),
    m_uvScale(1.0f, 1.0f),
    m_fresnal(1.0f, 0.5f, 2.0f),
    m_shallowColour(1.0f, 1.0f, 1.0f, 0.5),
    m_deepColour(0.8f, 0.8f, 0.8f, 1.0f),
    m_reflectionTint(1.0f, 1.0f, 1.0f),
    m_reflection(1.0f)
{
    m_backfacecull = false;
    m_waves.resize(Wave::MAX);

    m_waves[0].amplitude = 1.0f;
    m_waves[0].frequency = 1.0f;
    m_waves[0].phase = 1.0f;
    m_waves[0].directionX = -1.0f;
    m_waves[0].directionZ = 0.0f;

    m_waves[1].amplitude = 0.5f;
    m_waves[1].frequency = 2.0f;
    m_waves[1].phase = 1.0f;
    m_waves[1].directionX = -0.7f;
    m_waves[1].directionZ = 0.7f;
}

bool Water::Initialise(float height, float spacing, int size)
{
    m_height = height;

    return CreateGrid(glm::vec2(1.0f, 1.0f), spacing, 
        size, size, false, false) && MeshData::Initialise();
}

const std::vector<Water::Wave>& Water::Waves() const
{
    return m_waves;
}

const glm::vec2& Water::BumpVelocity() const
{
    return m_bumpVelocity;
}

float Water::Speed() const
{
    return m_speed;
}

const glm::vec2& Water::UVScale() const
{
    return m_uvScale;
}

const glm::vec3& Water::Fresnal() const
{
    return m_fresnal;
}

const glm::vec4& Water::Shallow() const
{
    return m_shallowColour;
}

const glm::vec4& Water::Deep() const
{
    return m_deepColour;
}

const glm::vec3& Water::ReflectionTint() const
{
    return m_reflectionTint;
}

float Water::Bump() const
{
    return m_bump;
}

float Water::ReflectionIntensity() const
{
    return m_reflection;
}

void Water::SetInstance(int index, const glm::vec2& position, bool flippedX, bool flippedZ)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
    m_instances[index].scale.x = flippedX ? -1.0f : 1.0f;
    m_instances[index].scale.z = flippedZ ? -1.0f : 1.0f;
}

void Water::AddInstance(const glm::vec2& position, bool flippedX, bool flippedZ)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), position, flippedX, flippedZ);
}