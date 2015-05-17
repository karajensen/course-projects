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
    * Shared values for creating patches
    * requires size of 3060 with fog 
    */
    const int PATCH_GRID_VERTICES = 35; 
    const int PATCH_ROCK_TYPES = 3;
    const int PATCH_ROCK_INSTANCES = 20;
    const float PATCH_GRID_SPACING = 10.0f;

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
           InitialiseShadows() &&
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
    sun->Specular(glm::vec3(0.0, 100.0/255.0, 1.0));
    sun->Specularity(15.0f);
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
    success &= InitialiseShader(Shader::ID_WATER, "water", Shader::NONE);
    success &= InitialiseShader(Shader::ID_PARTICLE, "particle", Shader::NONE);
    success &= InitialiseShader(Shader::ID_SHADOW, "shadow", Shader::NONE);
    success &= InitialiseShader(Shader::ID_FLAT, "flat", Shader::FLAT);
    success &= InitialiseShader(Shader::ID_BUMP, "bump", Shader::BUMP);
    success &= InitialiseShader(Shader::ID_SPECULAR, "specular", Shader::SPECULAR);
    success &= InitialiseShader(Shader::ID_BUMP_SPEC, "bumpspecular", Shader::SPECULAR|Shader::BUMP);
    success &= InitialiseShader(Shader::ID_DIFFUSE_CAUSTICS, "diffusecaustics", Shader::CAUSTICS);
    success &= InitialiseShader(Shader::ID_BUMP_CAUSTICS, "bumpcaustics", Shader::CAUSTICS|Shader::BUMP);
    
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
    success &= InitialiseTexture("coral1", "coral1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral2", "coral2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral_specular", "coral_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("coral_normal", "coral_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top1", "flowerA_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top2", "flowerA_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top_specular", "flowerA_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerA_top_normal", "flowerA_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top1", "flowerB_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top2", "flowerB_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top_specular", "flowerB_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerB_top_normal", "flowerB_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top1", "flowerC_top1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top2", "flowerC_top2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top_specular", "flowerC_top_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flowerC_top_normal", "flowerC_top_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot", "flower_bot.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_specular", "flower_bot_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_normal", "flower_bot_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large", "flower_bot_large.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large_specular", "flower_bot_large_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("flower_bot_large_normal", "flower_bot_large_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf", "leaf.png", Texture::FROM_FILE);
    success &= InitialiseTexture("leaf_specular", "leaf_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell1", "shell1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shell2", "shell2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish1", "starfish1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish2", "starfish2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("starfish_normal", "starfish_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin1", "urchin1.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin2", "urchin2.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin_normal", "urchin_normal.png", Texture::FROM_FILE);
    success &= InitialiseTexture("urchin_specular", "urchin_specular.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock", "rock.png", Texture::FROM_FILE);
    success &= InitialiseTexture("rock_bump", "rock_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand", "sand.png", Texture::FROM_FILE);
    success &= InitialiseTexture("sand_bump", "sand_bump.png", Texture::FROM_FILE);
    success &= InitialiseTexture("shadow", "shadow.png", Texture::FROM_FILE);

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

    for (int i = 0; i < PATCH_ROCK_TYPES; ++i)
    {
        for (int j = 0; j < PATCH_ROCK_INSTANCES; ++j)
        {
            const auto index = m_data.rocks.size();
            m_data.rocks.emplace_back();
            m_data.rocks[index].index = m_data.terrain.size();
            m_data.rocks[index].instance = j;
        }

        const auto ID(std::to_string(i));
        Terrain& rock = InitialiseTerrain("rock" + ID, "terrain" + ID, 
			Shader::ID_BUMP_CAUSTICS, 4.0f, false, 0.0f, -5.0f, 40.0f, 5.0f, 30);

        rock.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "rock"));
        rock.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "rock_bump"));
        rock.SetTexture(MeshData::CAUSTICS, causticsTexture);
        rock.AddInstances(PATCH_ROCK_INSTANCES);
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

bool SceneBuilder::InitialiseShadows()
{
    m_data.shadows = std::make_unique<Quad>("shadow", Shader::ID_SHADOW);
    m_data.shadows->SetTexture(MeshData::COLOUR, GetID(m_data.textures, "shadow"));
    return m_data.shadows->Initialise();
}

bool SceneBuilder::InitialiseMeshes()
{
    bool success = true;
    const int instances = 100;
    const int causticsTexture = m_data.animation[Animation::ID_CAUSTICS]->GetFrame();

    {
        auto& mesh = InitialiseMesh("skybox", "skybox.obj", 1.0f, 1.0f, Shader::ID_FLAT);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "skybox"));
        mesh.BackfaceCull(false);
        mesh.Ambience(0.85f);
        mesh.SetSkyBox();
        mesh.AddInstances(1);
    }
    {
        auto& mesh = InitialiseMesh("seaweed1", "seaweed1.obj", 0.25f, 4.0f, Shader::ID_SPECULAR);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("seaweed2", "seaweed2.obj", 0.2f, 8.0f, Shader::ID_SPECULAR);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("seaweed3", "seaweed3.obj", 0.2f, 8.0f, Shader::ID_SPECULAR);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "leaf"));
        mesh.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "leaf_specular"));
        mesh.BackfaceCull(false);
        mesh.Diffuse(0.3f); // Ensures backfaces are lit
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, false, instances);
    }
    {
        auto& mesh = InitialiseMesh("shell", "shell.obj", 2.0f, 4.0f, Shader::ID_DIFFUSE_CAUSTICS);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "shell1"));
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "shell2"));
        mesh.SetTexture(MeshData::CAUSTICS, causticsTexture);
        mesh.CausticsScale(0.1f);
        mesh.CausticsAmount(0.2f);
        mesh.Specular(0.3f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("starfish", "starfish.obj", 0.5f, 0.5f, Shader::ID_BUMP);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "starfish1"));
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "starfish2"));
        mesh.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "starfish_normal"));
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("urchin", "urchin.obj", 1.0f, 1.0f, Shader::ID_BUMP_SPEC);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "urchin1"));
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "urchin2"));
        mesh.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "urchin_normal"));
        mesh.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "urchin_specular"));
        mesh.Specular(0.5f);
        mesh.Bump(8.0f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh = InitialiseMesh("coral", "coral.obj", 1.0f, 4.0f, Shader::ID_BUMP_SPEC);
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "coral1"));
        mesh.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "coral2"));
        mesh.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "coral_normal"));
        mesh.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "coral_specular"));
        mesh.Bump(2.0f);
        mesh.Specular(0.5f);
        mesh.AddInstances(instances);
        success &= AddFoliage({ &mesh }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower1_top", "flower1_top.obj", 1.0f, 1.0f, Shader::ID_BUMP_SPEC);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerA_top1"));
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerA_top2"));
        mesh1.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flowerA_top_normal"));
        mesh1.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flowerA_top_specular"));
        mesh1.Bump(2.5f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower1_bot", "flower1_bot.obj", 2.0f, 2.0f, Shader::ID_BUMP_SPEC);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower_bot"));
        mesh2.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flower_bot_normal"));
        mesh2.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flower_bot_specular"));
        mesh2.AddInstances(instances);
        mesh2.Bump(2.5f);
        mesh2.Specular(0.4f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower2_top", "flower2_top.obj", 1.0f, 1.0f, Shader::ID_BUMP_SPEC);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerB_top1"));
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerB_top2"));
        mesh1.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flowerB_top_normal"));
        mesh1.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flowerB_top_specular"));
        mesh1.Bump(2.0f);
        mesh1.Specularity(5.0f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);
    
        auto& mesh2 = InitialiseMesh("flower2_bot", "flower2_bot.obj", 3.0f, 3.0f, Shader::ID_BUMP_SPEC);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower_bot_large"));
        mesh2.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flower_bot_large_normal"));
        mesh2.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flower_bot_large_specular"));
        mesh2.AddInstances(instances);
        mesh2.Specular(0.5f);
        mesh2.Bump(3.5f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
    }
    {
        auto& mesh1 = InitialiseMesh("flower3_top", "flower3_top.obj", 1.0f, 1.0f, Shader::ID_BUMP_SPEC);
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerC_top1"));
        mesh1.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flowerC_top2"));
        mesh1.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flowerC_top_normal"));
        mesh1.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flowerC_top_specular"));
        mesh1.Bump(2.5f);
        mesh1.Specular(0.5f);
        mesh1.AddInstances(instances);

        auto& mesh2 = InitialiseMesh("flower3_bot", "flower3_bot.obj", 1.0f, 2.0f, Shader::ID_BUMP_SPEC);
        mesh2.SetTexture(MeshData::COLOUR, GetID(m_data.textures, "flower_bot"));
        mesh2.SetTexture(MeshData::NORMAL, GetID(m_data.textures, "flower_bot_normal"));
        mesh2.SetTexture(MeshData::SPECULAR, GetID(m_data.textures, "flower_bot_specular"));
        mesh2.AddInstances(instances);
        mesh2.Bump(2.5f);
        mesh2.Specular(0.4f);
        success &= AddFoliage({ &mesh1, &mesh2 }, true, instances);
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
    data.instances = 120;
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
        !m_data.shaders[shaderID]->HasComponent(Shader::FLAT),
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP)))
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
        maxHeight, height, spacing, size,
		!m_data.shaders[shaderID]->HasComponent(Shader::FLAT),
        m_data.shaders[shaderID]->HasComponent(Shader::BUMP), tiling))
    {
        LogError("Terrain: " + name + " failed initialisation");
    }
    return terrain;
}

bool SceneBuilder::AddFoliage(std::initializer_list<const MeshData*> meshes, 
                              bool hasShadow, 
                              int instances)
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

        if (hasShadow)
        {
            const int shadowID = static_cast<int>(m_data.shadows->Instances().size());
            m_data.shadows->AddInstances(1);
            m_data.foliage[index].AddShadow(shadowID);
        }

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