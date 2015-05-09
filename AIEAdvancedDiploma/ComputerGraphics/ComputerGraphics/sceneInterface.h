////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneInterface.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

class Mesh;
class Shader;
class Texture;
class Light;
class PostProcessing;
class Terrain;
class Water;
class Emitter;

/**
* Allows access to the elements of the scene
*/
class IScene
{
public:

    virtual ~IScene() = default;

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<std::unique_ptr<Mesh>>& Meshes() const = 0;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<std::unique_ptr<Shader>>& Shaders() const = 0;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<std::unique_ptr<Light>>& Lights() const = 0;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<std::unique_ptr<Texture>>& Textures() const = 0;

    /**
    * @return the terrain in the scene
    */
    virtual const std::vector<std::unique_ptr<Terrain>>& Terrains() const = 0;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<std::unique_ptr<Emitter>>& Emitters() const = 0;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<std::unique_ptr<Water>>& Waters() const = 0;

    /**
    * @return the data for post processing the scene
    */
    virtual const PostProcessing& Post() const = 0;

    /**
    * Retrieves a shader at the given index
    * @param index The index of the shader to get
    * @return the shader 
    */
    virtual Shader& GetShader(int index) const = 0;

};                     