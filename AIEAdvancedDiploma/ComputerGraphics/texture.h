////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "renderdata.h"

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of image this texture is displaying
    */
    enum Type
    {
        FROM_FILE,
        CUBE,
        RANDOM
    };

    /**
    * Special textures that are not attached to a mesh 
    */
    enum Index
    {
        ID_RANDOM,
        MAX_TEXTURES
    };

    /**
    * Constructor for a texture loaded from file
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param type The type of image to display
    */
    Texture(const std::string& name, 
            const std::string& path,
            Type type);

    /**
    * Constructor for a procedurally generated texture
    * @param name The filename of the texture
    * @param path The full path to save the texture
    * @param size The dimensions of the texture
    * @param type The type of texture to make
    */
    Texture(const std::string& name, 
            const std::string& path,
            int size,
            Type type);

    /**
    * Destructor
    */
    ~Texture();

    /**
    * Initialises the texture
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

    /**
    * @return whether this texture is procedurally generated
    */
    bool IsProcedural() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the unique ID for the texture
    */
    GLuint GetID() const;

    /**
    * Reloads the texture from pixels
    * @return whether reloading was successful
    */
    bool ReloadPixels();

    /**
    * Saves a texture to file from its pixels
    * @return whether saving was successful
    */
    bool SaveTexture();

private:

    /**
    * Prevent copying
    */
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    /**
    * Creates a texture of random normals used for ambient occlusion
    */
    void MakeRandomNormals();

    /**
    * Initialises a cube map
    * @return whether initialisation was successful
    */
    bool InitialiseCubeMap();

    /**
    * Initialises the texture from file
    * @return whether initialisation was successful
    */
    bool InitialiseFromFile();

    /**
    * Initialises the texture from pixels
    * @return whether initialisation was successful
    */
    bool InitialiseFromPixels();

    /**
    * Loads a texture from file
    * @param type The type of texture to load
    * @param path The path to the texture
    * @return whether loading was successful
    */
    bool LoadTexture(GLenum type, const std::string& path);

    Type m_image;                       ///< The type of image displayed
    std::vector<unsigned int> m_pixels; ///< Optional pixels of the texture
    int m_size = 0;                     ///< Optional Dimensions of the texture
    bool m_initialised = false;         ///< Whether this texture is initialised
    bool m_cubeMap = false;             ///< Whether this texture is a cube map
    GLuint m_id = 0;                    ///< Unique id for the texture
    std::string m_name;                 ///< Name of the texture
    std::string m_path;                 ///< Path to the texture
};