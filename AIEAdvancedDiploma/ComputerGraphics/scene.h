////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sceneInterface.h"
#include "postprocessing.h"
#include <vector>
#include <string>
#include <memory>

class Camera;
class SceneBuilder;
class ScenePlacer;
struct SceneData;

/**
* Manager and owner of all object
*/
class Scene : public IScene
{
public:

    Scene();
    ~Scene();

    /**
    * Ticks the scene
    * @param deltatime The time passed between ticks
    * @param camera For obtaining view information
    */
    void Tick(float deltatime, const Camera& camera);

    /**
    * Renders the GUI manipulator
    */
    void RenderGUI();

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<std::unique_ptr<Mesh>>& Meshes() const override;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<std::unique_ptr<Shader>>& Shaders() const override;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<std::unique_ptr<Light>>& Lights() const override;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<std::unique_ptr<Texture>>& Textures() const override;

    /**
    * @return the terrain in the scene
    */
    virtual const std::vector<std::unique_ptr<Terrain>>& Terrains() const override;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<std::unique_ptr<Water>>& Waters() const override;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<std::unique_ptr<Emitter>>& Emitters() const override;

    /**
    * @return the data for post processing the scene
    */
    virtual const PostProcessing& Post() const override;

    /**
    * @param index The index of the shader to get
    * @return the shader 
    */
    virtual Shader& GetShader(int index) const override;

    /**
    * Sets which post map will currently be rendered
    * @param map Which map to render
    */
    void SetPostMap(PostProcessing::Map map);

    /**
    * Saves all procedurally generated textures to file
    */
    void SaveTextures();

    /**
    * Initialises the scene
    * @param camera The position of the camera
    * @return whether initialisation was successful
    */
    bool Initialise(const glm::vec3& camera);

    /**
    * Reloads the scene
    */
    void Reload();

    /**
    * @return the data for the scene
    */
    SceneData& GetData();

    /**
    * @return the placement updater for the scene
    */
    ScenePlacer& GetPlacer();

private:

    /**
    * Prevent copying
    */
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    std::unique_ptr<SceneData> m_data;         ///< Elements of the scene
    std::unique_ptr<SceneBuilder> m_builder;   ///< Creates meshes, lighting and shader data
    std::unique_ptr<ScenePlacer> m_placer;     ///< Updates the scene depending on the camera
}; 