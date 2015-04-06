////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "sceneData.h"
#include "sceneBuilder.h"
#include "sceneUpdater.h"

Scene::Scene() :
    m_data(std::make_unique<SceneData>())
{
}

Scene::~Scene() = default;

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

    const auto caustics = m_data->animation[Animation::ID_CAUSTICS]->GetFrame();

    for (auto& mesh : m_data->meshes)
    {
        if (GetShader(mesh->ShaderID()).HasComponent(Shader::CAUSTICS))
        {
            mesh->SetTexture(Mesh::CAUSTICS, caustics);
        }
    }

    for (auto& terrain : m_data->terrain)
    {
        if (GetShader(terrain->ShaderID()).HasComponent(Shader::CAUSTICS))
        {
            terrain->SetTexture(Mesh::CAUSTICS, caustics);
        }
    }

    // Temporary to test scene
    static float timePassed = 0.0f;
    timePassed += deltatime;
    m_data->meshes[0]->Instances()[0].position.y += cos(timePassed) * 0.05f;
}

bool Scene::Initialise(const glm::vec3& camera)
{
    m_builder = std::make_unique<SceneBuilder>(*m_data);
    m_updater = std::make_unique<SceneUpdater>(*m_data);

    if (m_builder->Initialise())
    {
        m_updater->Initialise(camera);

        // To prevent unnecessary shader switching, sort by shader used
        std::sort(m_data->meshes.begin(), m_data->meshes.end(), 
            [](const std::unique_ptr<Mesh>& m1, const std::unique_ptr<Mesh>& m2)->bool
            {
                return m1->ShaderID() < m2->ShaderID();
            });

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
            texture->Save();
        }
    }
}

void Scene::Reload()
{
    for (auto& texture : m_data->textures)
    {
        if (texture->IsProcedural())
        {
            texture->Reload();
        }
    }

    for (auto& terrain : m_data->terrain)
    {
        terrain->Reload();
    }
}