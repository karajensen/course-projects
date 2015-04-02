////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sceneInterface.h"
#include "postprocessing.h"
#include <vector>
#include <string>
#include <memory>

class ProceduralTexture;
class Animation;
class SceneBuilder;
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
    */
    void Tick(float deltatime);

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
    * Adds a shader to the scene
    * @param index The index to add the light at
    * @param element The element to add to the scene
    */
    void Add(int index, std::unique_ptr<Shader> element);

    /**
    * Adds an animation to the scene
    * @param index The index to add the animation at
    * @param element The element to add to the scene
    * @return the animation added
    */
    Animation& Add(int index, std::unique_ptr<Animation> element);

    /**
    * Adds a light to the scene
    * @param index The index to add the light at
    * @param element The element to add to the scene
    * @return the light added
    */
    Light& Add(int index, std::unique_ptr<Light> element);

    /**
    * Adds terrain to the scene
    * @param element The element to add to the scene
    * @return terrain added
    */
    Terrain& Add(std::unique_ptr<Terrain> element);

    /**
    * Adds water to the scene
    * @param element The element to add to the scene
    * @return the water added
    */
    Water& Add(std::unique_ptr<Water> element);

    /**
    * Adds an emitter to the scene
    * @param element The element to add to the scene
    * @return the emitter added
    */
    Emitter& Add(std::unique_ptr<Emitter> element);

    /**
    * Adds a mesh to the scene
    * @param element The element to add to the scene
    * @return the mesh added
    */
    Mesh& Add(std::unique_ptr<Mesh> element);

    /**
    * Adds a texture to the scene
    * @param element The element to add to the scene
    * @return the index the texture was added at
    */
    unsigned int Add(std::unique_ptr<Texture> element);

    /**
    * Adds a texture to the scene
    * @param element The element to add to the scene
    * @param index The optional index to add the texture at
    * @return the index the texture was added at
    */
    ProceduralTexture& Add(std::unique_ptr<ProceduralTexture> element, int index = NO_INDEX);

    /**
    * @param index The index of the shader to get
    * @return the shader 
    */
    virtual Shader& GetShader(int index) const override;

    /**
    * Retrieves a texture at the given index
    * @param index The index of the texture to get
    * @return the texture 
    */
    virtual const Texture& GetTexture(int index) const;

    /**
    * @param index The index of the texture to get
    * @return the texture 
    */
    Texture& GetTexture(int index);

    /**
    * Gets the texture index from the name
    */
    int GetTexture(const std::string& name) const;

    /**
    * @param name The name of the texture to get
    * @return the texture 
    */
    const ProceduralTexture& GetProceduralTexture(const std::string& name) const;

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
    * @return whether initialisation was successful
    */
    bool Initialise();

private:

    /**
    * Prevent copying
    */
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    std::unique_ptr<SceneData> m_data;         ///< Elements of the scene
    std::unique_ptr<SceneBuilder> m_builder;   ///< Creates meshes, lighting and shader data
};                     