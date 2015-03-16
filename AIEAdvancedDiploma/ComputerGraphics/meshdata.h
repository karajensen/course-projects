////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "renderdata.h"

/**
* Base Mesh Information
*/
class MeshData
{
public:

    /**
    * Type of texture to send to the mesh shader
    * Ordering must match usage in shader body
    */
    enum Slot
    {
        COLOUR,
        NORMAL,
        SPECULAR,
        ENVIRONMENT,
        CAUSTICS,
        MAX_TYPES
    };


    /**
    * Constructor
    * @param name The name of the data
    * @param shader The ID of the shader to use
    */
    MeshData(const std::string& name, int shader);

    /**
    * Destructor
    */
    virtual ~MeshData();

    /**
    * Initialises the buffers for the mesh
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * Pre-renders the mesh
    */
    void PreRender() const;

    /**
    * Renders the mesh
    */
    void Render() const;

    /**
    * @return The name of the mesh
    */
    const std::string& Name() const;

    /**
    * @return The ID of the shader to render with
    */
    int ShaderID() const;

    /**
    * @return The vertices constructing this mesh
    */
    const std::vector<float>& Vertices() const;

    /**
    * @return The indicies constructing this mesh
    */
    const std::vector<unsigned long>& Indices() const;

    /**
    * @return The ID for each texture type used
    */
    const std::vector<int>& TextureIDs() const;

    /**
    * @return Number of components that make up a vertex
    */
    int VertexComponentCount() const;

    /**
    * Sets the ID of the texture to use
    * @param type The type of texture to set
    * @param ID The ID of the texture to use
    */
    void SetTexture(Slot type, int ID);

    /**
    * Sets the ID of the textures to use
    * @param diffuse the ID for the diffuse texture
    * @param normal the ID for the normal texture
    * @param specular the ID for the specular texture
    */
    void SetTextures(int diffuse, 
                     int normal = NO_INDEX,
                     int specular = NO_INDEX);

protected:

    /**
    * Prevent copying
    */
    MeshData(const MeshData&) = delete;
    MeshData& operator=(const MeshData&) = delete;

    /**
    * Loads a grid into the buffers
    * @param position The center of the grid
    * @param spacing The spacing between vertices
    * @param rows How many rows for the grid
    * @param columns How many columns for the grid
    */
    void CreateGrid(const glm::vec3& position, float spacing, int rows, int columns);

    int m_vertexComponentCount = 0;   ///< Number of components that make up a vertex
    std::vector<float> m_vertices;    ///< Mesh Vertex information
    std::vector<DWORD> m_indices;     ///< Mesh Index information
    const std::string m_name;         ///< Name of the mesh
    int m_shaderIndex = -1;           ///< Unique Index of the mesh shader to use
    GLuint m_vaoID = 0;               ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID = 0;               ///< Unique ID for the Vertex Buffer Object (VBO)   
    GLuint m_iboID = 0;               ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;       ///< Whether the vertex buffer object is initialised or not
    std::vector<int> m_textureIDs;    ///< IDs for each texture used
};