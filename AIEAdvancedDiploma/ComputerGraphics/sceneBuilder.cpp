////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneBuilder.h"
#include "scene.h"
#include "mesh.h"
#include "shader.h"
#include "light.h"
#include "texture.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "animation.h"
#include <assert.h>

namespace
{
    /**
    * Resource file information
    */
    const std::string ASSETS_PATH(".//Assets//");
    const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
    const std::string MESHES_PATH(ASSETS_PATH + "Meshes//");
    const std::string TEXTURE_PATH(ASSETS_PATH + "Textures//");
    const std::string GENERATED_TEXTURES(TEXTURE_PATH + "Generated//");
}

SceneBuilder::SceneBuilder(Scene& scene) :
    m_scene(scene)
{
}

SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise()
{
    return InitialiseLighting() &&
           InitialiseTextures() &&
           InitialiseShaders() &&
           InitialiseEmitters() &&
           InitialiseWater() &&
           InitialiseMeshes();
}

bool SceneBuilder::InitialiseLighting()
{
    m_scene.Add(Light::ID_SUN, std::make_unique<Light>("Sun"));
    auto& sun = m_scene.GetLight(Light::ID_SUN);
    sun.Position(glm::vec3(0.0, 12.0, 0.0));
    sun.Attenuation(glm::vec3(1.3, 0.0, 0.0));
    sun.Diffuse(glm::vec3(1.0, 1.0, 1.0));
    sun.Specular(glm::vec3(0.0, 0.0, 0.0));
    sun.Specularity(1.0f);

    m_scene.Add(Light::ID_SPOT, std::make_unique<Light>("Spot"));
    auto& spot = m_scene.GetLight(Light::ID_SPOT);
    spot.Position(glm::vec3(0.0, 6.0, 0.0));
    spot.Attenuation(glm::vec3(0.0, 0.0, 0.08));
    spot.Diffuse(glm::vec3(0.0, 1.0, 0.0));
    spot.Specular(glm::vec3(1.0, 0.0, 0.0));
    spot.Specularity(1.0f);

    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    bool failed = false;

    failed |= !InitialiseShader(Shader::ID_POST_PROCESSING, "post_effects", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_PRE_PROCESSING, "pre_effects", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_BLUR_VERTICAL, "blur_vertical", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_BLUR_HORIZONTAL, "blur_horizontal", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_DIFFUSE, "diffuse", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_WATER, "water", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_PARTICLE, "particle", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_BUMP, "bump", Shader::BUMP);

    failed |= !InitialiseShader(Shader::ID_DIFFUSE_CAUSTICS, "diffusecaustics", Shader::CAUSTICS);

    failed |= !InitialiseShader(Shader::ID_FLAT, "flat", Shader::NONE);

    failed |= !InitialiseShader(Shader::ID_BUMP_SPEC_CAUSTICS, "bumpspecularcaustics", 
                                Shader::BUMP|Shader::SPECULAR|Shader::CAUSTICS);
    return !failed;
}

bool SceneBuilder::InitialiseTextures()
{
    return InitialiseTexture("random", "random.bmp", Texture::RANDOM, 128, Texture::ID_RANDOM) &&
        InitialiseTexture("water_environment", "water_environment", Texture::CUBE) &&
        InitialiseTexture("water_colour", "water.png", Texture::FROM_FILE) &&
        InitialiseTexture("water_normal", "water_normal.png", Texture::FROM_FILE) &&
        InitialiseTexture("particle1", "particle1.png", Texture::FROM_FILE) &&
        InitialiseTexture("particle2", "particle2.png", Texture::FROM_FILE) &&
        InitialiseTexture("specular", "specular.jpg", Texture::FROM_FILE) &&
        InitialiseTexture("specular", "specular.jpg", Texture::FROM_FILE) &&
        InitialiseTexture("ground", "ground.png", Texture::FROM_FILE) &&
        InitialiseTexture("bump", "bump.png", Texture::FROM_FILE) &&
        InitialiseTexture("bridge", "bridge.bmp", Texture::FROM_FILE) &&
        InitialiseTexture("sky", "sky.png", Texture::FROM_FILE) &&
        InitialiseTexture("blank", "blank.png", Texture::FROM_FILE) &&
        InitialiseCaustics();
}

bool SceneBuilder::InitialiseMeshes()
{	
    auto index = InitialiseMesh("sphereFBX", "sphere.fbx", Shader::ID_BUMP_SPEC_CAUSTICS);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.Bump(20.0f);
        mesh.Specularity(5.0f);
        mesh.SetTextures(m_scene.GetTexture("ground"), 
            m_scene.GetTexture("bump"), m_scene.GetTexture("specular"));
    }

    index = InitialiseMesh("sphereOBJ", "sphere.obj", Shader::ID_DIFFUSE_CAUSTICS);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.SetTextures(m_scene.GetTexture("blank"));
    }

    index = InitialiseMesh("bridge", "mock_bridge.obj", Shader::ID_DIFFUSE);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.SetTextures(m_scene.GetTexture("bridge"));
        mesh.BackfaceCull(false);
    }
    
    index = InitialiseMesh("sky", "mock_skybox.obj", Shader::ID_FLAT);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.SetTextures(m_scene.GetTexture("sky"));
        mesh.BackfaceCull(false);
    }
    
    index = InitialiseMesh("ground1", "mock_ground1.obj", Shader::ID_BUMP);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.Bump(20.0f);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
    }
    
    index = InitialiseMesh("ground2", "mock_ground2.obj", Shader::ID_BUMP);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.Bump(20.0f);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
    }
    
    index = InitialiseMesh("hill1", "mock_hill1.obj", Shader::ID_BUMP);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.Bump(20.0f);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
    }
    
    index = InitialiseMesh("hill2", "mock_hill2.obj", Shader::ID_BUMP);
    if (index != NO_INDEX)
    {
        auto& mesh = m_scene.GetMesh(index);
        mesh.Bump(20.0f);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
    }

    return true;
}

bool SceneBuilder::InitialiseEmitters()
{
    return InitialiseBubbles();
}

bool SceneBuilder::InitialiseBubbles()
{
    std::vector<int> textures = 
    {
        m_scene.GetTexture("particle1"),
        m_scene.GetTexture("particle2")
    };

    EmitterData data;
    data.direction.x = 0.0f;
    data.direction.y = 1.0f;
    data.direction.z = 0.0f;
    data.position.x = 0.0f;
    data.position.y = 0.0f;
    data.position.z = 0.0f;
    data.length = 10.0f;
    data.width = 10.0f;
    data.lifeTime = 2.0f;
    data.lifeFade = 0.25f;
    data.maxAmplitude = 1.0f;
    data.minAmplitude = 1.0f;
    data.maxFrequency = 1.0f;
    data.minFrequency = 1.0f;
    data.maxWaveSpeed = 1.0f;
    data.minWaveSpeed = 1.0f;
    data.maxSpeed = 0.5f;
    data.minSpeed = 0.2f;
    data.tint.r = 0.0f;
    data.tint.g = 0.5f;
    data.tint.b = 1.0f;
    data.tint.a = 1.0f;

    return InitialiseEmitter("test", Shader::ID_PARTICLE, 10, textures, data);
}

bool SceneBuilder::InitialiseWater()
{
    const auto index = m_scene.Add(std::make_unique<Water>("water", Shader::ID_WATER));

    Water& water = m_scene.GetWater(index);
    water.SetTexture(MeshData::COLOUR, m_scene.GetTexture("water_colour"));
    water.SetTexture(MeshData::NORMAL, m_scene.GetTexture("water_normal"));
    water.SetTexture(MeshData::ENVIRONMENT, m_scene.GetTexture("water_environment"));
    
    return water.Initialise(glm::vec3(0,-5,0), 10.0f, 10, 20);
}

bool SceneBuilder::InitialiseCaustics()
{
    m_scene.Add(Animation::ID_CAUSTICS, std::make_unique<Animation>());
    Animation& caustics = m_scene.GetAnimation(Animation::ID_CAUSTICS);

    const std::string path(TEXTURE_PATH + "Caustics//");
    const std::string name("Caustics_0");
    const std::string extension(".bmp");

    const int maxFrames = 40;
    for (int frame = 1; frame <= maxFrames; ++frame)
    {
        const std::string number(std::to_string(frame));
        const std::string frameName(name + (frame < 10 ? "0" : "") + number + extension);

        const auto index = m_scene.Add(std::make_unique<Texture>(
            frameName, path + frameName, Texture::FROM_FILE));
        
        caustics.AddFrame(index);
        
        if (!m_scene.GetTexture(index).Initialise())
        {
            return false;
        }
    }

    LogInfo("Caustics: Initialised " + std::to_string(maxFrames) + " frames");
    return true;
}

bool SceneBuilder::InitialiseShader(int index, 
                                    const std::string& name, 
                                    unsigned int components)
{
    m_scene.Add(index, std::make_unique<Shader>(name, SHADER_PATH + name, components));
    return m_scene.GetShader(index).Initialise();
}

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     const std::string& path,
                                     Texture::Type type)
{
    const auto index = m_scene.Add(std::make_unique<Texture>(name, TEXTURE_PATH + path, type));
    return m_scene.GetTexture(index).Initialise();
}

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     const std::string& path,
                                     Texture::Type type,
                                     int size,
                                     int index)
{
    m_scene.Add(index, std::make_unique<Texture>(
        name, GENERATED_TEXTURES + path, size, type));

    return m_scene.GetTexture(index).Initialise();
}

int SceneBuilder::InitialiseMesh(const std::string& name,
                                 const std::string& filename,
                                 int shaderID,
                                 int instances)
{
    const auto index = m_scene.Add(std::make_unique<Mesh>(name, shaderID, instances));

    if (m_scene.GetMesh(index).InitialiseFromFile(MESHES_PATH + filename, 
        true, true, m_scene.GetShader(shaderID).HasComponent(Shader::BUMP)))
    {
        return index;
    }

    LogError("Mesh: " + name + " failed initialisation");
    return NO_INDEX;
}

bool SceneBuilder::InitialiseEmitter(const std::string& name,
                                     int shaderID,
                                     int amount,
                                     const std::vector<int>& textures,
                                     const EmitterData& data)
{
    const auto index = m_scene.Add(std::make_unique<Emitter>(name, shaderID, amount));

    for (int texture : textures)
    {
        m_scene.GetEmitter(index).AddTexture(texture);
    }

    return m_scene.GetEmitter(index).Initialise(data);
}