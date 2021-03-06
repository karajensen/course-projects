﻿////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "emitter.h"
#include "common.h"
#include "quad.h"
#include "tweaker.h"

Emitter::Emitter(const std::string& name, int shaderID) :
    m_shaderIndex(shaderID),
    m_name(name),
    m_particle(std::make_unique<Quad>(name, shaderID))
{
}

Emitter::~Emitter() = default;

void Emitter::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", m_name);
    tweaker.AddIntEntry("Instances", [this](){ return m_instances.size(); });
    tweaker.AddEntry("Particles", &m_totalParticles, TW_TYPE_INT32, true);
    tweaker.AddEntry("Visible", &m_visibleInstances, TW_TYPE_INT32, true);
    tweaker.AddEntry("Paused", &m_paused, TW_TYPE_BOOLCPP);
    tweaker.AddFltEntry("Radius", &m_data.radius, 0.1f, 0.1f, FLT_MAX);
    tweaker.AddFltEntry("Length", &m_data.length, 0.1f, 0.1f, FLT_MAX);
    tweaker.AddFltEntry("Width", &m_data.width, 0.1f, 0.1f, FLT_MAX);
    tweaker.AddFltEntry("Direction", &m_data.direction.y, 0.1f);
    tweaker.AddFltEntry("Life Time", &m_data.lifeTime, 0.1f, 0.0f, FLT_MAX);
    tweaker.AddFltEntry("Life Fade", &m_data.lifeFade, 0.1f, 0.0f, FLT_MAX);
    tweaker.AddFltEntry("Min Wait Time", &m_data.minWaitTime, 0.1f, 0.0f, FLT_MAX);
    tweaker.AddFltEntry("Max Wait Time", &m_data.maxWaitTime, 0.1f, 0.0f, FLT_MAX);
    tweaker.AddFltEntry("Min Amplitude", &m_data.minAmplitude, 0.1f);
    tweaker.AddFltEntry("Max Amplitude", &m_data.maxAmplitude, 0.1f);
    tweaker.AddFltEntry("Min Frequency", &m_data.minFrequency, 0.1f);
    tweaker.AddFltEntry("Max Frequency", &m_data.maxFrequency, 0.1f);
    tweaker.AddFltEntry("Min Size", &m_data.minSize, 0.1f);
    tweaker.AddFltEntry("Max Size", &m_data.maxSize, 0.1f);
    tweaker.AddFltEntry("Min Speed", &m_data.minSpeed, 0.1f);
    tweaker.AddFltEntry("Max Speed", &m_data.maxSpeed, 0.1f);
    tweaker.AddEntry("Tint Colour", &m_data.tint, TW_TYPE_COLOR4F);
}

bool Emitter::Initialise(const EmitterData& data)
{
    m_data = data;
    m_data.radius = std::max(m_data.length, m_data.width);
    m_instances.resize(data.instances);
    m_totalParticles = data.instances * data.particles;

    for (Instance& instance : m_instances)
    {
        instance.particles.resize(data.particles);
    }

    return m_particle->Initialise();
}

const Emitter::Instance& Emitter::GetInstance(int index) const
{
    return m_instances[index];
}

unsigned int Emitter::InstanceCount() const
{
    return m_instances.size();
}

void Emitter::SetInstance(int index, const glm::vec3& position)
{
    m_instances[index].position = position;
}

void Emitter::PreRender()
{
    m_particle->PreRender();
}

void Emitter::Render(RenderParticle renderParticle,
                     const glm::vec3& cameraPosition,
                     const glm::vec3& cameraUp)
{
    glm::mat4 scale, rotate, translate;

    for (const Instance& instance : m_instances)
    {
        if (instance.render)
        {
            for (const Particle& particle : instance.particles)
            {
                if (particle.Alive())
                {
                    // Particle always facing the camera
                    glm::vec3 right, up, forward;
                    forward.x = cameraPosition.x - particle.Position().x;
                    forward.y = cameraPosition.y - particle.Position().y;
                    forward.z = cameraPosition.z - particle.Position().z;

                    forward = glm::normalize(forward);
                    right = glm::cross(forward, cameraUp);
                    up = glm::cross(forward, right);

                    scale[0][0] = particle.Size();
                    scale[1][1] = particle.Size();
                    scale[2][2] = particle.Size();

                    rotate[0][0] = right.x;
                    rotate[0][1] = right.y;
                    rotate[0][2] = right.z;
                    rotate[1][0] = up.x;
                    rotate[1][1] = up.y;
                    rotate[1][2] = up.z;
                    rotate[2][0] = forward.x;
                    rotate[2][1] = forward.y;
                    rotate[2][2] = forward.z;

                    translate[3][0] = particle.Position().x;
                    translate[3][1] = particle.Position().y;
                    translate[3][2] = particle.Position().z;

                    renderParticle(translate * rotate * scale, particle);
                    m_particle->Render();
                }
            }
        }
    }
}

void Emitter::TogglePaused()
{
    m_paused = !m_paused;
}

void Emitter::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

const std::string& Emitter::Name() const
{
    return m_name;
}

const std::vector<int>& Emitter::Textures() const
{
    return m_textures;
}

int Emitter::ShaderID() const
{
    return m_shaderIndex;
}

const glm::vec4& Emitter::Tint() const
{
    return m_data.tint;
}

void Emitter::AddTexture(int ID)
{
    if (ID == NO_INDEX)
    {
        LogError("Texture ID invalid");
    }

    m_textures.push_back(ID);
}

bool Emitter::ShouldRender(const glm::vec3& instancePosition,
                           const BoundingArea& bounds)
{
    // Radius requires a buffer as particles can move outside bounds
    m_data.radius = std::max(m_data.width, m_data.length) * m_data.maxAmplitude * 2.0f;
    const glm::vec3 centerToMesh = instancePosition - bounds.center;
    return glm::length(centerToMesh) <= m_data.radius + bounds.radius;
}

void Emitter::Tick(float deltatime,
                   const BoundingArea& cameraBounds)
{
    if (m_paused || !m_enabled)
    {
        return;
    }

    m_visibleInstances = 0;
    for (Instance& instance : m_instances)
    {
        instance.render = ShouldRender(instance.position, cameraBounds);
        if (instance.render)
        {
            ++m_visibleInstances;

            for (Particle& particle : instance.particles)
            {
                if (!particle.Tick(deltatime, m_data.direction))
                {
                     glm::vec3 particlePosition(instance.position);
                     particlePosition.x += Random::Generate(-m_data.width, m_data.width) * 0.5f;
                     particlePosition.z += Random::Generate(-m_data.length, m_data.length) * 0.5f;
    
                     const int textureID = m_textures[Random::Generate(
                         0, static_cast<int>(m_textures.size()-1))];
    
                     particle.Reset(m_data.lifeTime, 
                                    m_data.lifeFade,
                                    Random::Generate(m_data.minWaitTime, m_data.maxWaitTime),
                                    Random::Generate(m_data.minSpeed, m_data.maxSpeed),
                                    Random::Generate(m_data.minSize, m_data.maxSize),
                                    Random::Generate(m_data.minAmplitude, m_data.maxAmplitude),
                                    Random::Generate(m_data.minFrequency, m_data.maxFrequency),
                                    textureID,
                                    particlePosition);
    
                }
            }
        }
    }
}
