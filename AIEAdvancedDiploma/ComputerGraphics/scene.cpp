////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "sceneBuilder.h"
#include "common.h"
#include "shader.h"
#include "mesh.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "texture.h"
#include "animation.h"
#include "textureProcedural.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    std::vector<std::unique_ptr<Shader>> shaders;      ///< All shaders in the scene
    std::vector<std::unique_ptr<Mesh>> meshes;         ///< All meshes in the scene
    std::vector<std::unique_ptr<Light>> lights;        ///< All lights in the scene
    std::vector<std::unique_ptr<Texture>> textures;    ///< All textures in the scene
    std::vector<std::unique_ptr<Terrain>> terrain;     ///< All terrain in the scene
    std::vector<std::unique_ptr<Water>> water;         ///< All terrain in the scene
    std::vector<std::unique_ptr<Emitter>> emitters;    ///< All terrain in the scene
    std::vector<std::unique_ptr<Animation>> animation; ///< Managers for animated textures
    std::unique_ptr<PostProcessing> post;              ///< Data for post processing
};

Scene::Scene() :
    m_data(std::make_unique<SceneData>())
{
    m_data->lights.resize(Light::MAX_LIGHTS);
    m_data->shaders.resize(Shader::MAX_SHADERS);
    m_data->animation.resize(Animation::MAX_ANIMATION);
    m_data->textures.resize(Texture::MAX_TEXTURES);
    m_data->post = std::make_unique<PostProcessing>();
}

Scene::~Scene() = default;

void Scene::Add(int index, std::unique_ptr<Shader> element)
{
    m_data->shaders[index] = std::move(element);
}

Light& Scene::Add(int index, std::unique_ptr<Light> element)
{
    m_data->lights[index] = std::move(element);
    return *m_data->lights[index];
}

Animation& Scene::Add(int index, std::unique_ptr<Animation> element)
{
    m_data->animation[index] = std::move(element);
    return *m_data->animation[index];
}

Terrain& Scene::Add(std::unique_ptr<Terrain> element)
{
    m_data->terrain.push_back(std::move(element));
    return *m_data->terrain[m_data->terrain.size()-1];
}

Water& Scene::Add(std::unique_ptr<Water> element)
{
    m_data->water.push_back(std::move(element));
    return *m_data->water[m_data->water.size()-1];
}

Emitter& Scene::Add(std::unique_ptr<Emitter> element)
{
    m_data->emitters.push_back(std::move(element));
    return *m_data->emitters[m_data->emitters.size()-1];
}

Mesh& Scene::Add(std::unique_ptr<Mesh> element)
{
    m_data->meshes.push_back(std::move(element));
    return *m_data->meshes[m_data->meshes.size()-1];
}

ProceduralTexture& Scene::Add(std::unique_ptr<ProceduralTexture> element, int index)
{
    if (index == NO_INDEX)
    {
        index = m_data->textures.size();
        m_data->textures.push_back(std::move(element));
    }
    else
    {
        m_data->textures[index] = std::move(element);
    }
    return static_cast<ProceduralTexture&>(*m_data->textures[index]);
}

unsigned int Scene::Add(std::unique_ptr<Texture> element)
{
    m_data->textures.push_back(std::move(element));
    return m_data->textures.size()-1;
}

const std::vector<std::unique_ptr<Mesh>>& Scene::Meshes() const
{
    return m_data->meshes;
}

const std::vector<std::unique_ptr<Shader>>& Scene::Shaders() const
{
    return m_data->shaders;
}

const std::vector<std::unique_ptr<Light>>& Scene::Lights() const
{
    return m_data->lights;
}

const std::vector<std::unique_ptr<Texture>>& Scene::Textures() const
{
    return m_data->textures;
}

const std::vector<std::unique_ptr<Terrain>>& Scene::Terrains() const
{
    return m_data->terrain;
}

const std::vector<std::unique_ptr<Water>>& Scene::Waters() const
{
    return m_data->water;
}

const std::vector<std::unique_ptr<Emitter>>& Scene::Emitters() const
{
    return m_data->emitters;
}

const PostProcessing& Scene::Post() const
{
    return *m_data->post;
}

Shader& Scene::GetShader(int index) const
{
    return *m_data->shaders[index];
}

const Texture& Scene::GetTexture(int index) const
{
    return *m_data->textures[index];
}

Texture& Scene::GetTexture(int index)
{
    return *m_data->textures[index];
}

int Scene::GetTexture(const std::string& name)
{
    for (unsigned int i = 0; i < m_data->textures.size(); ++i)
    {
        if (IsStrEqual(name, m_data->textures[i]->Name()))
        {
            return i;
        }
    }

    LogError("Could not find texture " + name);
    return NO_INDEX;
}

void Scene::Tick(float deltatime)
{
    for (auto& emitter : m_data->emitters)
    {
        emitter->Tick(deltatime);
    }

    for (auto& animation : m_data->animation)
    {
        animation->Tick(deltatime);
    }

    for (auto& mesh : m_data->meshes)
    {
        if (GetShader(mesh->ShaderID()).HasComponent(Shader::CAUSTICS))
        {
            mesh->SetTexture(Mesh::CAUSTICS, 
                m_data->animation[Animation::ID_CAUSTICS]->GetFrame());
        }
    }

    // Temporary to test scene
    static float timePassed = 0.0f;
    timePassed += deltatime;
    m_data->meshes[0]->Instances()[0].position.y += cos(timePassed) * 0.05f;
}

bool Scene::Initialise()
{
    m_builder = std::make_unique<SceneBuilder>(*this);

    if (m_builder->Initialise())
    {
        // To prevent unnecessary shader switching, sort by shader used
        //std::sort(m_data->meshes.begin(), m_data->meshes.end(), 
        //    [](const std::unique_ptr<Mesh>& m1, const std::unique_ptr<Mesh>& m2)->bool
        //    {
        //        return m1->ShaderID() < m2->ShaderID();
        //    });

        LogInfo("Scene: Successfully created");
        return true;
    }
    return false;
}

void Scene::SetPostMap(PostProcessing::Map map)
{
    LogInfo("Rendering: " + PostProcessing::GetMapName(map));
    m_data->post->SetPostMap(map);
}

void Scene::SaveTextures()
{
    for (auto& texture : m_data->textures)
    {
        if (texture->IsProcedural())
        {
            static_cast<ProceduralTexture&>(*texture).SaveTexture();
        }
    }
}