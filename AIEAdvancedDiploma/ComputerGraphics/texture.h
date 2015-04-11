////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "renderdata.h"

class Tweaker;

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
        PROCEDURAL
    };

    /**
    * Type of filtering for this texture
    */
    enum Filter
    {
        NEAREST,
        LINEAR,
        ANISOTROPIC
    };

    /**
    * Constructor for a texture
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param type The type of image to display
    * @param filter The type of filtering for this texture
    */
    Texture(const std::string& name, 
            const std::string& path,
            Type type,
            Filter filter);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Destructor
    */
    virtual ~Texture();

    /**
    * Initialises the texture
    * @return whether initialisation was successful
    */
    virtual bool Initialise();

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
    * @return the path of the texture
    */
    const std::string& Path() const;

    /**
    * @return the unique ID for the texture
    */
    GLuint GetID() const;

    /**
    * Reloads the texture
    * @throw not implemented exception
    */
    virtual void Reload();

    /**
    * Saves a texture to file from its pixels
    * @throw not implemented exception
    */
    virtual void Save();

    /**
    * @return the pixels of the texture
    * @throw not implemented exception
    */
    virtual const std::vector<unsigned int>& GetPixels() const;

protected:

    /**
    * Sets the filtering for the texture
    * @note must be called after any texture is generated
    * @return whether setting was successful
    */
    bool SetFiltering();

private:

    /**
    * Prevent copying
    */
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    /**
    * Creates the mipmaps for the texture
    * @return whether creation was successful
    */
    bool CreateMipMaps();

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
    * @throw not implemented exception
    */
    virtual bool InitialiseFromPixels();

    /**
    * Loads a texture from file
    * @param type The type of opengl texture to load
    * @param path The path to the texture
    * @return whether loading was successful
    */
    bool LoadTexture(GLenum type, const std::string& path);

    Filter m_filter;             ///< The type of filtering for this texture
    Type m_image;                ///< The type of image displayed
    bool m_initialised = false;  ///< Whether this texture is initialised
    GLuint m_id = 0;             ///< Unique id for the texture
    std::string m_name;          ///< Name of the texture
    std::string m_path;          ///< Path to the texture
};