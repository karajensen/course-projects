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
    * Callbacks for rendering a mesh instance
    */
    typedef std::function<void(const glm::mat4&)> RenderInstance;

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        glm::vec3 position = glm::vec3(0,0,0); ///< Position offset
        glm::vec3 rotation = glm::vec3(0,0,0); ///< Degress rotated around each axis
        glm::vec3 scale = glm::vec3(1,1,1);    ///< Scaling of the mesh
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
    * Reloads the buffers for the mesh
    * @return whether reloadig was successful
    */
    bool Reload();

    /**
    * Pre-renders the mesh
    */
    void PreRender() const;

    /**
    * Renders the mesh
    */
    void Render() const;

    /**
    * Renders the mesh
    * @param renderInstance Callback to render a single mesh instance
    */
    void Render(RenderInstance renderInstance) const;

    /**
    * @return The name of the mesh
    */
    const std::string& Name() const;

    /**
    * @return The ID of the shader to render with
    */
    int ShaderID() const;

    /**
    * @return The instances of this mesh
    */
    const std::vector<Instance>& Instances() const;

    /**
    * @return The instances of this mesh
    */
    std::vector<Instance>& Instances();

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
    * @return Whether back facing polygons are culled
    */
    bool BackfaceCull() const;

    /**
    * Sets Whether back facing polygons are culled
    */
    void BackfaceCull(bool value);

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

    /**
    * Adds an instance for this mesh
    * @param position The position offset 
    * @param rotation How much to rotate 
    * @param scale The size of the mesh
    */
    void AddInstance(const glm::vec3& position,
                     const glm::vec3& rotation,
                     float scale);

    /**
    * Adds an instance at the world center with default values
    */
    void AddInstance();

    /**
    * Sets an instance for this mesh
    * @param index The ID of this instance
    * @param position The position offset 
    * @param rotation How much to rotate 
    * @param scale The size of the mesh
    */
    void SetInstance(int index,
                     const glm::vec3& position,
                     const glm::vec3& rotation,
                     float scale);
protected:

    /**
    * Prevent copying
    */
    MeshData(const MeshData&) = delete;
    MeshData& operator=(const MeshData&) = delete;

    bool m_backfacecull = true;         ///< Whether backface culling is enabled
    int m_vertexComponentCount = 0;     ///< Number of components that make up a vertex
    std::vector<float> m_vertices;      ///< Mesh Vertex information
    std::vector<DWORD> m_indices;       ///< Mesh Index information
    const std::string m_name;           ///< Name of the mesh
    int m_shaderIndex = -1;             ///< Unique Index of the mesh shader to use
    GLuint m_vaoID = 0;                 ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID = 0;                 ///< Unique ID for the Vertex Buffer Object (VBO)   
    GLuint m_iboID = 0;                 ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;         ///< Whether the vertex buffer object is initialised or not
    std::vector<int> m_textureIDs;      ///< IDs for each texture used
    std::vector<Instance> m_instances;  ///< Instances of this mesh
};