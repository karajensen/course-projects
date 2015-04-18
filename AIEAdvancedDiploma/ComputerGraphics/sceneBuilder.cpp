////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneBuilder.h"
#include "sceneData.h"
#include "rendertarget.h"
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

    /**
    * Helper function to get a texture by name
    */
    int GetTexture(const SceneData& data, const std::string& name)
    {
        for (unsigned int i = 0; i < data.textures.size(); ++i)
        {
            if (IsStrEqual(name, data.textures[i]->Name()))
            {
                return i;
            }
        }
        LogError("Could not find texture " + name);
        return NO_INDEX;
    }
}

SceneBuilder::SceneBuilder(SceneData& data) :
    m_data(data)
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
    m_data.lights.resize(Light::MAX_LIGHTS);
    
    auto& sun = m_data.lights[Light::ID_SUN];
    sun = std::make_unique<Light>("Sun");
    sun->Position(glm::vec3(0.0, 12.0, 0.0));
    sun->Attenuation(glm::vec3(1.3, 0.0, 0.0));
    sun->Diffuse(glm::vec3(1.0, 1.0, 1.0));
    sun->Specular(glm::vec3(0.0, 0.0, 0.0));
    sun->Specularity(1.0f);

    auto& spot = m_data.lights[Light::ID_SPOT];
    spot = std::make_unique<Light>("Spot");
    spot->Position(glm::vec3(0.0, 6.0, 0.0));
    spot->Attenuation(glm::vec3(0.0, 0.0, 0.08));
    spot->Diffuse(glm::vec3(0.0, 1.0, 0.0));
    spot->Specular(glm::vec3(1.0, 0.0, 0.0));
    spot->Specularity(1.0f);

    return true;
}

bool SceneBuilder::InitialiseShaderConstants()
{
    const auto& post = *m_data.post;
    Shader::ShaderConstants constants = 
    {
        std::make_pair("MAX_LIGHTS", std::to_string(Light::MAX_LIGHTS)),
        std::make_pair("MAX_WAVES", std::to_string(Water::Wave::MAX)),
        std::make_pair("WINDOW_WIDTH", std::to_string(WINDOW_WIDTH)),
        std::make_pair("WINDOW_HEIGHT", std::to_string(WINDOW_HEIGHT)),
        std::make_pair("SAMPLES", std::to_string(MULTISAMPLING_COUNT)),
        std::make_pair("SCENE_TEXTURES", std::to_string(RenderTarget::SCENE_TEXTURES)),
        std::make_pair("ID_COLOUR", std::to_string(RenderTarget::SCENE_ID)),
        std::make_pair("ID_NORMAL", std::to_string(RenderTarget::NORMAL_ID)),
        std::make_pair("WEIGHT0", std::to_string(post.BlurWeight(0))),
        std::make_pair("WEIGHT1", std::to_string(post.BlurWeight(1))),
        std::make_pair("WEIGHT2", std::to_string(post.BlurWeight(2))),
        std::make_pair("WEIGHT3", std::to_string(post.BlurWeight(3))),
        std::make_pair("WEIGHT4", std::to_string(post.BlurWeight(4))),
    };

    Shader::InitialiseConstants(constants);
    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    bool success = true;
    m_data.shaders.resize(Shader::MAX_SHADERS);

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

    success &= InitialiseShader(Shader::ID_BUMP_CAUSTICS, "bumpcaustics", 
                                Shader::CAUSTICS|Shader::BUMP);

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
    success &= InitialiseTexture("blank", "blank.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sky", "sky.png", Texture::FROM_FILE);

    {
        auto& texture = InitialiseTexture("heightmap", Texture::NEAREST,
            ProceduralTexture::DIAMOND_SQUARE, 256);
        success &= texture.Initialise();
    }
    
    return success && InitialiseCaustics();
}

bool SceneBuilder::InitialiseTerrain()
{
    const int causticsTexture = 
        m_data.animation[Animation::ID_CAUSTICS]->GetFrame();

    {
        m_data.sandIndex = m_data.terrain.size();
        Terrain& terrain = InitialiseTerrain("sand", "heightmap",
            Shader::ID_BUMP_CAUSTICS, glm::vec2(0.25f, 0.25f), -25.0f, 0.0f, 0.5f, 10.0f, 51);
        terrain.SetTexture(MeshData::COLOUR, GetTexture(m_data, "blank"));
        terrain.SetTexture(MeshData::NORMAL, GetTexture(m_data, "bump"));
        terrain.SetTexture(MeshData::CAUSTICS, causticsTexture);
        terrain.Bump(20.0f);
        terrain.CausticsAmount(0.5f);
    }

    return true;
}

bool SceneBuilder::InitialiseWater()
{
    const auto index = m_data.water.size();
    m_data.water.push_back(std::make_unique<Water>(
        "water", m_data.shaders[Shader::ID_WATER]->Name(), Shader::ID_WATER));
    auto& water = *m_data.water[index];

    water.SetTexture(MeshData::COLOUR, GetTexture(m_data, "water_colour"));
    water.SetTexture(MeshData::NORMAL, GetTexture(m_data, "water_normal"));
    water.SetTexture(MeshData::ENVIRONMENT, GetTexture(m_data, "water_cube"));
    return water.Initialise(25.0f, 10.0f, 51);
}

bool SceneBuilder::InitialiseMeshes()
{
    const int causticsTexture = 
        m_data.animation[Animation::ID_CAUSTICS]->GetFrame();

    {
        auto& mesh = InitialiseMesh("skybox", "mock_skybox.obj", Shader::ID_FLAT);
        mesh.SetTexture(MeshData::COLOUR, GetTexture(m_data, "sky"));
        mesh.SetSkyBox();
    }
    {
        auto& mesh = InitialiseMesh("sphere1", "sphere.obj", Shader::ID_BUMP_SPEC_CAUSTICS);
        mesh.SetTexture(MeshData::COLOUR, GetTexture(m_data, "ground"));
        mesh.SetTexture(MeshData::NORMAL, GetTexture(m_data, "bump"));
        mesh.SetTexture(MeshData::SPECULAR, GetTexture(m_data, "specular"));
        mesh.SetTexture(MeshData::CAUSTICS, causticsTexture);
        mesh.SetInstance(0, glm::vec3(0, 5, 0));
        mesh.Bump(20.0f);
        mesh.Specularity(5.0f);
    }
    {
        auto& mesh = InitialiseMesh("sphere2", "sphere.obj", Shader::ID_DIFFUSE_CAUSTICS);
        mesh.SetTexture(MeshData::COLOUR, GetTexture(m_data, "blank"));
        mesh.SetTexture(MeshData::CAUSTICS, causticsTexture);
    }
    {
        auto& mesh = InitialiseMesh("cube", "cube.fbx", Shader::ID_DIFFUSE_CAUSTICS);
        mesh.SetTexture(MeshData::COLOUR, GetTexture(m_data, "water_colour"));
        mesh.SetTexture(MeshData::CAUSTICS, causticsTexture);
        mesh.SetInstance(0, glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), 2.0f);
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
        GetTexture(m_data, "particle1"),
        GetTexture(m_data, "particle2")
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

bool SceneBuilder::InitialiseCaustics()
{
    m_data.animation.resize(Animation::MAX_ANIMATION);

    auto& caustics = m_data.animation[Animation::ID_CAUSTICS];
    caustics = std::make_unique<Animation>();

    const std::string path("Caustics//");
    const std::string name("Caustics_0");
    const std::string extension(".bmp");

    const int maxFrames = 45;
    for (int frame = 1; frame <= maxFrames; ++frame)
    {
        const std::string number(std::to_string(frame));
        const std::string frameName(name + (frame < 10 ? "0" : "") + number + extension);
        caustics->AddFrame(static_cast<int>(m_data.textures.size()));

        if (!InitialiseTexture(frameName, path + frameName, Texture::FROM_FILE))
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
    m_data.shaders[index] = std::make_unique<Shader>(name, SHADER_PATH + name, components);
    return m_data.shaders[index]->Initialise();
}

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     const std::string& path,
                                     Texture::Type type,
                                     Texture::Filter filter)
{
    const auto index = m_data.textures.size();
    m_data.textures.push_back(std::make_unique<Texture>(name, TEXTURE_PATH + path, type, filter));
    return m_data.textures[index]->Initialise();
}

ProceduralTexture& SceneBuilder::InitialiseTexture(const std::string& name, 
                                                   Texture::Filter filter,
                                                   ProceduralTexture::Type type,
                                                   int size)
{
    const auto index = m_data.textures.size();
    m_data.proceduralTextures.push_back(index);

    m_data.textures.push_back(std::make_unique<ProceduralTexture>(
            name, GENERATED_TEXTURES + name + ".bmp", size, filter, type));

    return static_cast<ProceduralTexture&>(*m_data.textures[index]);
}

Mesh& SceneBuilder::InitialiseMesh(const std::string& name,
                                   const std::string& filename,
                                   int shaderID,
                                   int instances)
{
    m_data.meshes.push_back(std::make_unique<Mesh>(
        name, m_data.shaders[shaderID]->Name(), shaderID, instances));
    auto& mesh = *m_data.meshes[m_data.meshes.size()-1];

    if (!mesh.InitialiseFromFile(MESHES_PATH + filename, 
        true, true, m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
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
    m_data.emitters.push_back(std::make_unique<Emitter>(name, shaderID, amount));
    Emitter& emitter = *m_data.emitters[m_data.emitters.size()-1];

    for (int texture : textures)
    {
        emitter.AddTexture(texture);
    }

    return emitter.Initialise(data);
}

Terrain& SceneBuilder::InitialiseTerrain(const std::string& name,
                                         const std::string& heightmap,
                                         int shaderID,
                                         const glm::vec2& uvStretch,
                                         float height,
                                         float minHeight,
                                         float maxHeight,
                                         float spacing,
                                         int size)
{
    const int ID = GetTexture(m_data, heightmap);
    if (ID == NO_INDEX)
    {
        LogError("Terrain: " + name + " could not find texture " + heightmap);
    }

    const auto& texture = *m_data.textures[ID];
    m_data.terrain.push_back(std::make_unique<Terrain>(
        name, m_data.shaders[shaderID]->Name(), shaderID, texture.GetPixels()));
    Terrain& terrain = *m_data.terrain[m_data.terrain.size()-1];

    if (!terrain.Initialise(uvStretch, minHeight, maxHeight, height,
        spacing, size, true, m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
    {
        LogError("Terrain: " + name + " failed initialisation");
    }
    return terrain;
}