////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "renderdata.h"
#include "sceneData.h"
#include "sceneBuilder.h"
#include "scenePlacer.h"
#include "camera.h"

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

void Scene::Tick(float deltatime, const Camera& camera)
{
    const int causticsTexture = 
        m_data->animation[Animation::ID_CAUSTICS]->GetFrame();

    const auto& position = camera.Position();
    const auto& bounds = camera.GetBounds();

    for (auto& emitter : m_data->emitters)
    {
        emitter->Tick(deltatime, bounds);
    }

    for (auto& animation : m_data->animation)
    {
        animation->Tick(deltatime);
    }

    for (auto& mesh : m_data->meshes)
    {
        mesh->Tick(position, bounds, causticsTexture);
    }

    for (auto& terrain : m_data->terrain)
    {
        terrain->Tick(position, bounds, causticsTexture);
    }

    for (auto& water : m_data->water)
    {
        water->Tick(position, bounds, causticsTexture);
    }

    m_placer->Update(position);
}

bool Scene::Initialise(const glm::vec3& camera)
{
    m_builder = std::make_unique<SceneBuilder>(*m_data);
    if (m_builder->Initialise())
    {
        m_placer = std::make_unique<ScenePlacer>(*m_data);
        return m_placer->Initialise(camera);
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

    m_placer->ResetPatches();
}

SceneData& Scene::GetData()
{
    return *m_data;
}

ScenePlacer& Scene::GetPlacer()
{
    return *m_placer;
}