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
    * Shared values for creating meshes
    */
    const int PATCH_GRID_VERTICES = 51;
    const int PATCH_ROCK_TYPES = 3;
    const float PATCH_GRID_SPACING = 8.0f;

    /**
    * Helper function to get an asset by name
    */
    template<typename T> 
    int GetID(const std::vector<T>& data, const std::string& name)
    {
        for (unsigned int i = 0; i < data.size(); ++i)
        {
            if (IsStrEqual(name, data[i]->Name()))
            {
                return i;
            }
        }
        LogError("Could not find " + name);
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
    sun->Position(glm::vec3(0.0, 60.0, 0.0));
    sun->Attenuation(glm::vec3(1.0, 0.0, 0.0));
    sun->Diffuse(glm::vec3(0.85, 0.85, 1.0));
    sun->Specular(glm::vec3(1.0, 1.0, 1.0));
    sun->Specularity(20.0f);

    //auto& spot = m_data.lights[Light::ID_SPOT];
    //spot = std::make_unique<Light>("Spot");
    //spot->Position(glm::vec3(0.0, 6.0, 0.0));
    //spot->Attenuation(glm::vec3(0.0, 0.0, 0.08));
    //spot->Diffuse(glm::vec3(0.0, 1.0, 0.0));
    //spot->Specular(glm::vec3(1.0, 0.0, 0.0));
    //spot->Specularity(1.0f);

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
        std::make_pair("ID_DEPTH", std::to_string(RenderTarget::DEPTH_ID)),
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
    success &= InitialiseTexture("bubble1", "bubble1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble2", "bubble2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("bubble3", "bubble3.png", Texture::FROM_FILE);
    success &= InitialiseTexture("skybox", "skybox.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral", "coral.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower2_top", "flower2_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower2_bot", "flower2_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower3_top", "flower3_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower3_bot", "flower3_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower1_top", "flower1_top.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower1_bot", "flower1_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf", "leaf.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell", "shell.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish", "starfish.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin", "urchin.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock", "rock.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_bump", "rock_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_specular", "rock_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand", "sand.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand_bump", "sand_bump.png", Texture::FROM_FILE);

    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        success &= InitialiseTexture("terrain" + 
            std::to_string(i), ProceduralTexture::PERLIN_NOISE_ROCK, 128);
    }

    success &= InitialiseTexture("sand_height", ProceduralTexture::FROM_FILE, 256);

    return success && InitialiseCaustics();
}

bool SceneBuilder::InitialiseTerrain()
{
    const int causticsTexture = 
        m_data.animation[Animation::ID_CAUSTICS]->GetFrame();

    m_data.sandIndex = m_data.terrain.size();
    Terrain& sand = InitialiseTerrain("sand", "sand_height", 
        Shader::ID_BUMP_CAUSTICS, 4.0f, true, -45.0f, 
        0.0f, 5.0f, PATCH_GRID_SPACING, PATCH_GRID_VERTICES);

    sand.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "sand"));
    sand.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "sand_bump"));
    sand.SetTexture(MeshData::CAUSTICS, causticsTexture);
    sand.Bump(10.0f);
    sand.Ambience(1.0f);
    sand.CausticsAmount(0.2f);

    const int instancesPerType = 10;
    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        for (int j = 0; j < instancesPerType; ++j)
        {
            const auto index = m_data.rocks.size();
            m_data.rocks.emplace_back();
            m_data.rocks[index].index = m_data.terrain.size();
            m_data.rocks[index].instance = j;
        }

        const auto ID(std::to_string(i));
        Terrain& rock = InitialiseTerrain("rock" + ID, "terrain" + ID, 
            Shader::ID_BUMP_CAUSTICS, 4.0f, false, 0.0f, -5.0f, 40.0f, 5.0f, 30);

        rock.AddInstances(instancesPerType);
        rock.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "rock"));
        rock.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "rock_bump"));
        rock.SetTexture(MeshData::CAUSTICS, causticsTexture);
        rock.Bump(15.0f);
        rock.CausticsAmount(0.8f);
    }

    return true;
}

bool SceneBuilder::InitialiseWater()
{
    const auto index = m_data.water.size();
    m_data.water.push_back(std::make_unique<Water>(
        "water", m_data.shaders[Shader::ID_WATER]->Name(), Shader::ID_WATER));
    auto& water = *m_data.water[index];

    water.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "water_colour"));
    water.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "water_normal"));
    water.SetTexture(MeshData::ENVIRONMENT, GetID(m_data.textures, "water_cube"));
    return water.Initialise(25.0f, PATCH_GRID_SPACING, PATCH_GRID_VERTICES);
}

bool SceneBuilder::InitialiseMeshes()
{
    bool success = true;

    const int causticsTexture = 
        m_data.animation[Animation::ID_CAUSTICS]->GetFrame();

    {
        auto& mesh = InitialiseMesh("skybox", "skybox.obj", 0.0f, 0.0f, Shader::ID_FLAT);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "skybox"));
        mesh.BackfaceCull(false);
        mesh.Ambience(0.85f);
        mesh.SetSkyBox();
        mesh.AddInstances(1);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("sealeaves", "sealeaves.obj", 0.25f, 4.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("seaweed1", "seaweed1.obj", 0.25f, 4.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("seaweed2", "seaweed2.obj", 0.2f, 8.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("shell", "shell.obj", 2.0f, 4.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "shell"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("starfish", "starfish.obj", 0.5f, 0.5f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "starfish"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("urchin", "urchin.obj", 1.0f, 1.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "urchin"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh = InitialiseMesh("coral", "coral.obj", 1.0f, 4.0f, Shader::ID_DIFFUSE);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "coral"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower1 top", "flower1_top.obj", 1.0f, 1.0f, Shader::ID_DIFFUSE);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower1_top"));
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower1 bot", "flower1_bot.obj", 2.0f, 2.0f, Shader::ID_DIFFUSE);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower1_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower2 top", "flower2_top.obj", 1.0f, 1.0f, Shader::ID_DIFFUSE);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower2_top"));
        mesh1.AddInstances(instances);
    
        auto& mesh2 = InitialiseMesh("flower2 bot", "flower2_bot.obj", 3.0f, 3.0f, Shader::ID_DIFFUSE);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower2_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }
    {
        const int instances = 80;
        auto& mesh1 = InitialiseMesh("flower3 top", "flower3_top.obj", 1.0f, 1.0f, Shader::ID_DIFFUSE);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower3_top"));
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower3 bot", "flower3_bot.obj", 1.0f, 2.0f, Shader::ID_DIFFUSE);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower3_bot"));
        mesh2.AddInstances(instances);
        success &= AddFoliage({ &mesh1, &mesh2 }, instances);
    }

    return success;
}

bool SceneBuilder::InitialiseEmitters()
{
    return InitialiseBubbles();
}

bool SceneBuilder::InitialiseBubbles()
{
    std::vector<int> textures = 
    {
        GetID(m_data.textures, "bubble1"),
        GetID(m_data.textures, "bubble2"),
        GetID(m_data.textures, "bubble3")
    };

    EmitterData data;
    data.direction.x = 0.0f;
    data.direction.y = 1.0f;
    data.direction.z = 0.0f;
    data.length = 10.0f;
    data.width = 10.0f;
    data.lifeTime = 5.0f;
    data.lifeFade = 0.5f;
    data.maxAmplitude = 1.5f;
    data.minAmplitude = 0.5f;
    data.maxFrequency = 1.0f;
    data.minFrequency = 0.5f;
    data.maxWaveSpeed = 1.0f;
    data.minWaveSpeed = 1.0f;
    data.maxSpeed = 0.3f;
    data.minSpeed = 0.2f;
    data.minSize = 0.5f;
    data.maxSize = 1.5f;
    data.minWaitTime = 0.5f;
    data.maxWaitTime = 3.0f;    
    data.tint.r = 1.0f;
    data.tint.g = 1.0f;
    data.tint.b = 1.0f;
    data.tint.a = 1.0f;
    data.instances = 100;
    data.particles = 15;

    return InitialiseEmitter("bubbles", Shader::ID_PARTICLE, textures, data);
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

bool SceneBuilder::InitialiseTexture(const std::string& name, 
                                     ProceduralTexture::Generation generation,
                                     int size)
{
    const auto index = m_data.textures.size();
    m_data.proceduralTextures.push_back(index);

    const std::string path(generation == ProceduralTexture::FROM_FILE ? 
        TEXTURE_PATH + name + ".png" : GENERATED_TEXTURES + name + ".bmp");

    m_data.textures.push_back(std::make_unique<ProceduralTexture>(
        name, path, size, generation));

    if (m_data.textures[index]->Initialise())
    {
        m_data.textures[index]->Save();
        return true;
    }
    return false;
}

Mesh& SceneBuilder::InitialiseMesh(const std::string& name,
                                   const std::string& filename,
                                   float uScale,
                                   float vScale,
                                   int shaderID)
{
    const auto index = m_data.meshes.size();

    m_data.meshes.push_back(std::make_unique<Mesh>(
        name, m_data.shaders[shaderID]->Name(), shaderID));

    auto& mesh = *m_data.meshes[index];

    if (!mesh.InitialiseFromFile(MESHES_PATH + filename, glm::vec2(uScale, vScale),
        true, m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
    {
        LogError("Mesh: " + name + " failed initialisation");
    }

    return mesh;
}

bool SceneBuilder::InitialiseEmitter(const std::string& name,
                                     int shaderID,
                                     const std::vector<int>& textures,
                                     const EmitterData& data)
{
    m_data.emitters.push_back(std::make_unique<Emitter>(name, shaderID));
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
                                         float uvTextureStretch,
                                         bool tiling,
                                         float height,
                                         float minHeight,
                                         float maxHeight,
                                         float spacing,
                                         int size)
{
    const int ID = GetID(m_data.textures, heightmap);
    if (ID == NO_INDEX)
    {
        LogError("Terrain: " + name + " could not find texture " + heightmap);
    }

    const auto& texture = *m_data.textures[ID];
    m_data.terrain.push_back(std::make_unique<Terrain>(
        name, m_data.shaders[shaderID]->Name(), shaderID, texture.GetPixels()));
    Terrain& terrain = *m_data.terrain[m_data.terrain.size()-1];

    if (!terrain.Initialise(uvTextureStretch, minHeight, 
        maxHeight, height, spacing, size, true, 
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP), tiling))
    {
        LogError("Terrain: " + name + " failed initialisation");
    }
    return terrain;
}

bool SceneBuilder::AddFoliage(std::initializer_list<const MeshData*> meshes, int instances)
{
    if (meshes.size() == 0)
    {
        LogError("Empty foliage mesh group");
        return false;
    }

    for (int i = 0; i < instances; ++i)
    {
        const auto index = m_data.foliage.size();
        m_data.foliage.emplace_back();

        for (const auto* mesh : meshes)
        {
            if (static_cast<int>(mesh->Instances().size()) != instances)
            {
                LogError("Mesh " + mesh->Name() + " did not have require instances");
                return false;
            }

            m_data.foliage[index].AddMesh(GetID(m_data.meshes, mesh->Name()), i);
        }
    }
    return true;
}