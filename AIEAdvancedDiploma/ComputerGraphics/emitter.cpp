////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "emitter.h"
#include "common.h"
#include "quad.h"
#include "tweaker.h"

Emitter::Emitter(const std::string& name, int shaderID, int amount) :
    m_shaderIndex(shaderID),
    m_name(name),
    m_particle(std::make_unique<Quad>(name, shaderID))
{
    m_particles.resize(amount);
}

Emitter::~Emitter() = default;

void Emitter::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", [this](){ return m_name; });
    tweaker.AddIntEntry("Particles", [this](){ return m_particles.size(); });
    tweaker.AddEntry("Visible", &m_render, TW_TYPE_BOOLCPP);
    tweaker.AddEntry("Paused", &m_paused, TW_TYPE_BOOLCPP);
    tweaker.AddEntry("Radius", &m_data.radius, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Length", &m_data.length, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Width", &m_data.width, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Position X", &m_data.position.x, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Position Y", &m_data.position.y, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Position Z", &m_data.position.z, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Direction", &m_data.direction.y, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Life Time", &m_data.lifeTime, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Life Fade", &m_data.lifeFade, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Min Amplitude", &m_data.minAmplitude, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Max Amplitude", &m_data.maxAmplitude, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Min Frequency", &m_data.minFrequency, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Max Frequency", &m_data.maxFrequency, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Min Size", &m_data.minSize, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Max Size", &m_data.maxSize, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Min Speed", &m_data.minSpeed, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Max Speed", &m_data.maxSpeed, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Min Wave Speed", &m_data.minWaveSpeed, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Max Wave Speed", &m_data.maxWaveSpeed, TW_TYPE_FLOAT, 0.1f);
    tweaker.AddEntry("Tint Colour", &m_data.tint, TW_TYPE_COLOR4F);
}

bool Emitter::Initialise(const EmitterData& data)
{
    m_data = data;
    return m_particle->Initialise();
}

void Emitter::PreRender()
{
    m_particle->PreRender();
}

void Emitter::Render(RenderParticle renderParticle,
                     const glm::vec3& cameraPosition,
                     const glm::vec3& cameraUp)
{
    for (const Particle& particle : Particles())
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

            glm::mat4 scale;
            scale[0][0] = particle.Size();  
            scale[1][1] = particle.Size();
            scale[2][2] = particle.Size();
            
            glm::mat4 rotate;
            rotate[0][0] = right.x;  
            rotate[0][1] = right.y;
            rotate[0][2] = right.z;
            rotate[1][0] = up.x;  
            rotate[1][1] = up.y;
            rotate[1][2] = up.z;
            rotate[2][0] = forward.x;  
            rotate[2][1] = forward.y;
            rotate[2][2] = forward.z;

            glm::mat4 translate;
            translate[3][0] = particle.Position().x;
            translate[3][1] = particle.Position().y;
            translate[3][2] = particle.Position().z;
            
            renderParticle(translate * rotate * scale, particle);
            m_particle->Render();
        }
    }
}

void Emitter::TogglePaused()
{
    m_paused = !m_paused;
}

const std::string& Emitter::Name() const
{
    return m_name;
}

const std::vector<int>& Emitter::Textures() const
{
    return m_textures;
}

const std::vector<Particle>& Emitter::Particles() const
{
    return m_particles;
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

bool Emitter::ShouldRender(const glm::vec3& position, 
                           const BoundingArea& bounds)
{
    // Radius requires a buffer as particles can move outside bounds
    m_data.radius = std::max(m_data.width, m_data.length) * m_data.maxAmplitude * 2.0f;
    const glm::vec3 centerToMesh = m_data.position - bounds.center;
    return glm::length(centerToMesh) <= m_data.radius + bounds.radius;
}

bool Emitter::ShouldRender() const
{
    return m_render;
}

void Emitter::Tick(float deltatime,
                   const glm::vec3& cameraPosition,
                   const BoundingArea& cameraBounds)
{
    m_render = ShouldRender(cameraPosition, cameraBounds);

    if (!m_render || m_paused)
    {
        return;
    }
    
    for (Particle& particle : m_particles)
    {
        if (!particle.Tick(deltatime, m_data.direction))
        {
             glm::vec3 particlePosition(m_data.position);
             particlePosition.x += Random::Generate(-m_data.width, m_data.width) * 0.5f;
             particlePosition.z += Random::Generate(-m_data.length, m_data.length) * 0.5f;
    
             const int textureID = m_textures[Random::Generate(
                 0, static_cast<int>(m_textures.size()-1))];
    
             particle.Reset(m_data.lifeTime, 
                            m_data.lifeFade,
                            Random::Generate(m_data.minSpeed, m_data.maxSpeed),
                            Random::Generate(m_data.minWaveSpeed, m_data.maxWaveSpeed),
                            Random::Generate(m_data.minSize, m_data.maxSize),
                            Random::Generate(m_data.minAmplitude, m_data.maxAmplitude),
                            Random::Generate(m_data.minFrequency, m_data.maxFrequency),
                            textureID,
                            particlePosition);
    
        }
    }
}
