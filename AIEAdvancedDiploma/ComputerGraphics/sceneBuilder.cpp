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
#include "rendertarget.h"
#include "textureProcedural.h"
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
           InitialiseShaderConstants() &&
           InitialiseShaders() &&
           InitialiseEmitters() &&
           InitialiseWater() &&
           InitialiseMeshes() && 
           InitialiseTerrain();
}

bool SceneBuilder::InitialiseLighting()
{
    auto& sun = m_scene.Add(Light::ID_SUN, std::make_unique<Light>("Sun"));
    sun.Position(glm::vec3(0.0, 12.0, 0.0));
    sun.Attenuation(glm::vec3(1.3, 0.0, 0.0));
    sun.Diffuse(glm::vec3(1.0, 1.0, 1.0));
    sun.Specular(glm::vec3(0.0, 0.0, 0.0));
    sun.Specularity(1.0f);

    auto& spot = m_scene.Add(Light::ID_SPOT, std::make_unique<Light>("Spot"));
    spot.Position(glm::vec3(0.0, 6.0, 0.0));
    spot.Attenuation(glm::vec3(0.0, 0.0, 0.08));
    spot.Diffuse(glm::vec3(0.0, 1.0, 0.0));
    spot.Specular(glm::vec3(1.0, 0.0, 0.0));
    spot.Specularity(1.0f);

    return true;
}

bool SceneBuilder::InitialiseShaderConstants()
{
    const auto& post = m_scene.Post();
    const float randomU = WINDOW_WIDTH / static_cast<float>(Texture::RANDOM_SIZE);
    const float randomV = WINDOW_HEIGHT / static_cast<float>(Texture::RANDOM_SIZE);

    Shader::ShaderConstants constants = 
    {
        std::make_pair("MAX_LIGHTS", std::to_string(Light::MAX_LIGHTS)),
        std::make_pair("MAX_WAVES", std::to_string(Water::Wave::MAX)),
        std::make_pair("WINDOW_WIDTH", std::to_string(WINDOW_WIDTH)),
        std::make_pair("WINDOW_HEIGHT", std::to_string(WINDOW_HEIGHT)),
        std::make_pair("SAMPLES", std::to_string(MULTISAMPLING_COUNT)),
        std::make_pair("SCENE_TEXTURES", std::to_string(RenderTarget::SCENE_TEXTURES)),
        std::make_pair("EFFECTS_TEXTURES", std::to_string(RenderTarget::EFFECTS_TEXTURES)),
        std::make_pair("ID_COLOUR", std::to_string(RenderTarget::SCENE_ID)),
        std::make_pair("ID_NORMAL", std::to_string(RenderTarget::NORMAL_ID)),
        std::make_pair("ID_EFFECTS", std::to_string(RenderTarget::EFFECTS_ID)),
        std::make_pair("WEIGHT0", std::to_string(post.BlurWeight(0))),
        std::make_pair("WEIGHT1", std::to_string(post.BlurWeight(1))),
        std::make_pair("WEIGHT2", std::to_string(post.BlurWeight(2))),
        std::make_pair("WEIGHT3", std::to_string(post.BlurWeight(3))),
        std::make_pair("WEIGHT4", std::to_string(post.BlurWeight(4))),
        std::make_pair("RANDOM_UVS", std::to_string(randomU) + "," + std::to_string(randomV))
    };

    Shader::InitialiseConstants(constants);
    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    bool success = true;

    success &= InitialiseShader(Shader::ID_POST_PROCESSING, "post_effects", Shader::NONE);

    success &= InitialiseShader(Shader::ID_PRE_PROCESSING, "pre_effects", Shader::NONE);

    success &= InitialiseShader(Shader::ID_BLUR_VERTICAL, "blur_vertical", Shader::NONE);

    success &= InitialiseShader(Shader::ID_BLUR_HORIZONTAL, "blur_horizontal", Shader::NONE);

    success &= InitialiseShader(Shader::ID_DIFFUSE, "diffuse", Shader::NONE);

    success &= InitialiseShader(Shader::ID_WATER, "water", Shader::NONE);

    success &= InitialiseShader(Shader::ID_PARTICLE, "particle", Shader::NONE);

    success &= InitialiseShader(Shader::ID_BUMP, "bump", Shader::BUMP);

    success &= InitialiseShader(Shader::ID_DIFFUSE_CAUSTICS, "diffusecaustics", Shader::CAUSTICS);

    success &= InitialiseShader(Shader::ID_FLAT, "flat", Shader::NONE);

    success &= InitialiseShader(Shader::ID_BUMP_SPEC_CAUSTICS, "bumpspecularcaustics", 
                                Shader::BUMP|Shader::SPECULAR|Shader::CAUSTICS);
    return success;
}

bool SceneBuilder::InitialiseTextures()
{
    bool success = true;

    success &= InitialiseTexture("water_cube", "water_environment", Texture::CUBE, Texture::ANISOTROPIC);
    success &= InitialiseTexture("water_colour", "water.png", Texture::FROM_FILE);
    success &= InitialiseTexture("water_normal", "water_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("particle1", "particle1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("particle2", "particle2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("specular", "specular.jpg", Texture::FROM_FILE);
    success &= InitialiseTexture("specular", "specular.jpg", Texture::FROM_FILE);
    success &= InitialiseTexture("ground", "ground.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bump", "bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bridge", "bridge.bmp", Texture::FROM_FILE);
    success &= InitialiseTexture("sky", "sky.png", Texture::FROM_FILE);
    success &= InitialiseTexture("blank", "blank.png", Texture::FROM_FILE);

    auto& random = InitialiseTexture("random", Texture::NEAREST, Texture::RANDOM_SIZE, Texture::ID_RANDOM);
    random.MakeRandomNormals();
    success &= random.Initialise();

    auto& test = InitialiseTexture("test", Texture::NEAREST, 256);
    test.MakeDiamondSquareFractal();
    success &= test.Initialise();
    
    return success && InitialiseCaustics();
}

bool SceneBuilder::InitialiseTerrain()
{
    bool success = true;

    Terrain& sand = m_scene.Add(std::make_unique<Terrain>("sand", Shader::ID_DIFFUSE));
    sand.SetTexture(MeshData::COLOUR, m_scene.GetTexture("ground"));
    success &= sand.Initialise(Terrain::DIAMOND_SQUARE, glm::vec3(0, -5, 0), 1.0f, 100);

    return success;
}

bool SceneBuilder::InitialiseMeshes()
{	
    {
        auto& mesh = InitialiseMesh("sphereFBX", "sphere.obj", Shader::ID_BUMP_SPEC_CAUSTICS);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"), m_scene.GetTexture("specular"));
        mesh.Bump(20.0f);
        mesh.Specularity(5.0f);
    }
    {
        auto& mesh = InitialiseMesh("sphereOBJ", "sphere.obj", Shader::ID_DIFFUSE_CAUSTICS);
        mesh.SetTextures(m_scene.GetTexture("blank"));
    }
    {
        auto& mesh = InitialiseMesh("bridge", "mock_bridge.obj", Shader::ID_DIFFUSE);
        mesh.SetTextures(m_scene.GetTexture("bridge"));
        mesh.BackfaceCull(false);
    }
    {
        auto& mesh = InitialiseMesh("sky", "mock_skybox.obj", Shader::ID_FLAT);
        mesh.SetTextures(m_scene.GetTexture("sky"));
        mesh.BackfaceCull(false);
    }
    {
        auto& mesh = InitialiseMesh("ground1", "mock_ground1.obj", Shader::ID_BUMP);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
        mesh.Bump(20.0f);
    }
    {
        auto& mesh = InitialiseMesh("ground2", "mock_ground2.obj", Shader::ID_BUMP);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
        mesh.Bump(20.0f);
    }
    {
        auto& mesh = InitialiseMesh("hill1", "mock_hill1.obj", Shader::ID_BUMP);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
        mesh.Bump(20.0f);
    }
    {
        auto& mesh = InitialiseMesh("hill2", "mock_hill2.obj", Shader::ID_BUMP);
        mesh.SetTextures(m_scene.GetTexture("ground"), m_scene.GetTexture("bump"));
        mesh.Bump(20.0f);
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
    bool success = true;

    Water& water = m_scene.Add(std::make_unique<Water>("water", Shader::ID_WATER));
    water.SetTexture(MeshData::COLOUR, m_scene.GetTexture("water_colour"));
    water.SetTexture(MeshData::NORMAL, m_scene.GetTexture("water_normal"));
    water.SetTexture(MeshData::ENVIRONMENT, m_scene.GetTexture("water_environment"));
    success &= water.Initialise(glm::vec3(0,5,0), 10.0f, 10, 20);

    return success;
}

bool SceneBuilder::InitialiseCaustics()
{
    Animation& caustics = m_scene.Add(Animation::ID_CAUSTICS, std::make_unique<Animation>());

    const std::string path(TEXTURE_PATH + "Caustics//");
    const std::string name("Caustics_0");
    const std::string extension(".bmp");

    const int maxFrames = 40;
    for (int frame = 1; frame <= maxFrames; ++frame)
    {
        const std::string number(std::to_string(frame));
        const std::string frameName(name + (frame < 10 ? "0" : "") + number + extension);

        const auto index = m_scene.Add(std::make_unique<Texture>(
            frameName, path + frameName, Texture::FROM_FILE, Texture::LINEAR));
        
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
                                     Texture::Type type,
                                     Texture::Filter filter)
{
    const auto index = m_scene.Add(std::make_unique<Texture>(name, TEXTURE_PATH + path, type, filter));
    return m_scene.GetTexture(index).Initialise();
}

ProceduralTexture& SceneBuilder::InitialiseTexture(const std::string& name, 
                                                   Texture::Filter filter,
                                                   int size,
                                                   int index)
{
    return m_scene.Add(std::make_unique<ProceduralTexture>(
            name, GENERATED_TEXTURES + name + ".bmp", size, filter), index);
}

Mesh& SceneBuilder::InitialiseMesh(const std::string& name,
                                   const std::string& filename,
                                   int shaderID,
                                   int instances)
{
    auto& mesh = m_scene.Add(std::make_unique<Mesh>(name, shaderID, instances));

    if (!mesh.InitialiseFromFile(MESHES_PATH + filename, 
        true, true, m_scene.GetShader(shaderID).HasComponent(Shader::BUMP)))
    {
        LogError("Mesh: " + name + " failed initialisation");
    }

    return mesh;
}

bool SceneBuilder::InitialiseEmitter(const std::string& name,
                                     int shaderID,
                                     int amount,
                                     const std::vector<int>& textures,
                                     const EmitterData& data)
{
    Emitter& emitter = m_scene.Add(std::make_unique<Emitter>(name, shaderID, amount));

    for (int texture : textures)
    {
        emitter.AddTexture(texture);
    }

    return emitter.Initialise(data);
}