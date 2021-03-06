////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "renderdata.h"
#include "particle.h"

class Quad;
class Tweaker;
struct BoundingArea;

/**
* Data for a particle emitter
*/
struct EmitterData
{
    float radius = 0.0f;          ///< The radius of the emitter
    float width = 0.0f;           ///< The width of the emitter
    float length = 0.0f;          ///< The length of the emitter
    float minSpeed = 1.0f;        ///< The minimum speed of the particles
    float maxSpeed = 1.0f;        ///< The maximum speed of the particles
    float minSize = 1.0f;         ///< The minimum size of the particles
    float maxSize = 1.0f;         ///< The maximum size of the particles
    float minAmplitude = 1.0f;    ///< The minimum height of the particle waves
    float maxAmplitude = 1.0f;    ///< The maximum height of the particle waves
    float minFrequency = 1.0f;    ///< The minimum intensity of the waves
    float maxFrequency = 1.0f;    ///< The maximum intensity of the waves
    float lifeTime = 0.0f;        ///< Seconds the particle can live before dying
    float lifeFade = 1.0f;        ///< Seconds before lifetime ends to fade the particle
    float minWaitTime = 0.0f;     ///< Seconds to wait before respawning
    float maxWaitTime = 0.0f;     ///< Seconds to wait before respawning
    glm::vec3 direction;          ///< The direction the particles will spawn 
    glm::vec4 tint;               ///< Colour to tint the particle texture
    int instances = 0;            ///< Number of instances of this emitter
    int particles = 0;            ///< Number of particles per instance
};

/**
* Manages all particles spawned
* @note currently only support for an emitter aligned with the ground plane
*/
class Emitter
{
public:

    /**
    * Callback for rendering a particle
    */
    typedef std::function<void(const glm::mat4&, const Particle&)> RenderParticle;

    /**
    * Constructor
    * @param name The name of the emitter
    * @param shaderID The ID of the shader to render with
    */
    Emitter(const std::string& name, int shaderID);

    /**
    * Destructor
    */
    ~Emitter();

    /**
    * Holds information for an instance of this emitter
    * @note instances have the same data but particles do not match
    */
    struct Instance
    {
        std::vector<Particle> particles;   ///< Particles this emitter can spawn
        bool render = true;                ///< Whether this emitter should be rendered
        glm::vec3 position;                ///< Position of this emitter
    };

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Initialises the emitter
    * @param data The data to initialise the emitter with
    * @return whether initialisation succeeded
    */
    bool Initialise(const EmitterData& data);

    /**
    * Ticks the emitter
    * @param deltatime The time passed between ticks
    * @param cameraBounds Bounding area in front of the camera
    */
    void Tick(float deltatime, 
              const BoundingArea& cameraBounds);

    /**
    * Pre Renders the emitter
    */
    void PreRender();

    /**
    * Renders the emitter
    * @param renderParticle Callback for rendering a single particle
    * @param cameraPosition The world position of the camera
    * @param camerUp The up vector of the camera
    */
    void Render(RenderParticle renderParticle,
                const glm::vec3& cameraPosition,
                const glm::vec3& cameraUp);

    /**
    * Toggles whether the emitter is paused
    */
    void TogglePaused();

    /**
    * Sets whether the emitter is enabled or not
    */
    void SetEnabled(bool enabled);

    /**
    * @return The name of the emitter
    */
    const std::string& Name() const;

    /**
    * @return The texture IDs used in the emitter
    */
    const std::vector<int>& Textures() const;

    /**
    * @return The shader used for rendering particles
    */
    int ShaderID() const;

    /**
    * @return The colour tint of the particles
    */
    const glm::vec4& Tint() const;

    /**
    * Adds a texture to be used by the particles
    * @param ID The unique ID of the texture to use
    */
    void AddTexture(int ID);    

    /**
    * @return the instance of the emitter
    */
    const Instance& GetInstance(int index) const;

    /**
    * Sets the position of the instance
    */
    void SetInstance(int index, const glm::vec3& position);

    /**
    * @return the amount of instances of this emitter
    */
    unsigned int InstanceCount() const;

private:

    /**
    * Prevent copying
    */
    Emitter(const Emitter&) = delete;
    Emitter& operator=(const Emitter&) = delete;

    /**
    * Determines whether the emitter should be rendered
    * @param instancePosition The position of the emitter instance
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const glm::vec3& instancePosition, 
                      const BoundingArea& bounds);

    EmitterData m_data;                  ///< Data for this emitter
    std::vector<int> m_textures;         ///< Indexes for the particle textures to use
    std::vector<Instance> m_instances;   ///< All instances of this emitter
    std::unique_ptr<Quad> m_particle;    ///< Particle quad for rendering
    int m_shaderIndex = -1;              ///< Unique Index of the mesh shader to render with
    int m_totalParticles = 0;            ///< Total amount of particles over all instances
    int m_visibleInstances = 0;          ///< Number of instances currently rendered
    std::string m_name;                  ///< Name of this emitter
    bool m_paused = false;               ///< Whether emission is paused
    bool m_enabled = false;              ///< Whether emission is enabled
};