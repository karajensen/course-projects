////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "texture.h"

class Scene;
class Mesh;
class Light;
class Shader;
class MeshData;
struct EmitterData;

/**
* Builds all objects and diagnostics for the scene
*/
class SceneBuilder
{
public:

    SceneBuilder(Scene& scene);
    ~SceneBuilder();

    /**
    * Initialises the scene
    * @return Whether the initialization was successful
    */
    bool Initialise();

private:

    /**
    * Prevent copying
    */
    SceneBuilder(const SceneBuilder&) = delete;
    SceneBuilder& operator=(const SceneBuilder&) = delete;

    /**
    * Initiliases all shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseShaders();

    /**
    * Initialises the lighting for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting();

    /**
    * Initialises all textures required
    * @return Whether the initialization was successful
    */
    bool InitialiseTextures();

    /**
    * Initialises the meshes for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes();

    /**
    * Initialises the water for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseWater();

    /**
    * Initialises the emitters for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseEmitters();

    /**
    * Initialises a mesh
    * @param name The name of the mesh
    * @param filename The filename of the mesh
    * @param shaderID The shader to use
    * @param instances The number of instances to create
    * @return The index of the created mesh or NO_INDEX if failed
    */
    int InitialiseMesh(const std::string& name,
                       const std::string& filename,
                       int shaderID,
                       int instances = 1);

    /**
    * Initialises a shader
    * @param index The index to add the shader at
    * @param name the filename of the shader
    * @param components What the shader is made up of
    * @return Whether the initialization was successful
    */
    bool InitialiseShader(int index, 
                          const std::string& name, 
                          unsigned int components);

    /**
    * Initialises the caustics animation
    * @return Whether the initialization was successful
    */
    bool InitialiseCaustics();

    /**
    * Initialises the bubble emitter
    * @return Whether the initialization was successful 
    */
    bool InitialiseBubbles();

    /**
    * Initialises a texture
    * @param name The name of the texture
    * @param path The path of the texture
    * @param type The image type of the texture
    * @return Whether the initialization was successful
    */
    bool InitialiseTexture(const std::string& name, 
                           const std::string& path,
                           Texture::Type type);

    /**
    * Initialises a procedural texture
    * @param name The name of the texture
    * @param path The path of the texture
    * @param type The image type of the texture
    * @param size The size of the texture
    * @param index The index to initialise the texture at
    * @return Whether the initialization was successful
    */
    bool InitialiseTexture(const std::string& name, 
                           const std::string& path,
                           Texture::Type type,
                           int size,
                           int index);

    /**
    * Initialises an emitter
    * @param name The name of the emitter
    * @param shader The id of the shader to use
    * @param amount The number of particles to emit
    * @param textures The particle textures to use
    * @param data The data for the emission
    */
    bool InitialiseEmitter(const std::string& name, 
                           int shaderID,
                           int amount,
                           const std::vector<int>& textures,
                           const EmitterData& data);

    Scene& m_scene; ///< The scene to build
};                     