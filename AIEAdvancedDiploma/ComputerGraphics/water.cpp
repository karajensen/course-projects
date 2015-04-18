////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "water.h"
#include "tweaker.h"

Water::Water(const std::string& name, 
             const std::string& shaderName, 
             int shaderID) :

    Grid(name, shaderName, shaderID),
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
    BackfaceCull(false);
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

void Water::AddToTweaker(Tweaker& tweaker)
{
    Grid::AddToTweaker(tweaker);

    tweaker.AddFltEntry("Height", [this](){ return m_height; }, [this](const float value)
    { 
        m_height = value;
        for (auto& instance : Instances())
        {
            instance.position.y = m_height;
        }
    });

    tweaker.AddFltEntry("Speed", &m_speed, 0.1f);
    tweaker.AddFltEntry("Bump Amount", &m_bump, 0.1f);
    tweaker.AddFltEntry("Bump Velocity X", &m_bumpVelocity.x, 0.1f);
    tweaker.AddFltEntry("Bump Velocity Z", &m_bumpVelocity.y, 0.1f);
    tweaker.AddFltEntry("Texture Scale U", &m_uvScale.x, 0.1f);
    tweaker.AddFltEntry("Texture Scale V", &m_uvScale.y, 0.1f);
    tweaker.AddFltEntry("Fresnal Scale", &m_fresnal.x, 0.1f);
    tweaker.AddFltEntry("Fresnal Biase", &m_fresnal.y, 0.1f);
    tweaker.AddFltEntry("Fresnal Power", &m_fresnal.z, 0.1f);
    tweaker.AddFltEntry("Reflection Amount", &m_reflection, 0.1f);
    tweaker.AddEntry("Reflection Tint", &m_reflectionTint, TW_TYPE_COLOR3F);
    tweaker.AddEntry("Shallow Colour", &m_shallowColour, TW_TYPE_COLOR4F);
    tweaker.AddEntry("Deep Colour", &m_deepColour, TW_TYPE_COLOR4F);

    for (unsigned int i = 0; i < m_waves.size(); ++i)
    {
        std::vector<std::pair<std::string, float*>> wave;
        wave.push_back(std::make_pair("Amplitude", &m_waves[i].amplitude));
        wave.push_back(std::make_pair("Frequency", &m_waves[i].frequency));
        wave.push_back(std::make_pair("Phase", &m_waves[i].phase));
        wave.push_back(std::make_pair("Direction X", &m_waves[i].directionX));
        wave.push_back(std::make_pair("Direction Z", &m_waves[i].directionZ));
        tweaker.AddSubGroup("Wave " + std::to_string(i), wave, 0.1f);
    }
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
    m_instances[index].requiresUpdate = true;
}

void Water::AddInstance(const glm::vec2& position, bool flippedX, bool flippedZ)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), position, flippedX, flippedZ);
}