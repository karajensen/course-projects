////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "renderdata.h"
#include "glm\glm.hpp"

class Tweaker;
struct BoundingArea;

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
        glm::mat4 world;                       ///< World matrix
        glm::vec3 position = glm::vec3(0,0,0); ///< Position offset
        glm::vec3 rotation = glm::vec3(0,0,0); ///< Degrees rotated around each axis
        glm::vec3 scale = glm::vec3(1,1,1);    ///< Scaling of the mesh
        bool render = true;                    ///< Whether to draw the mesh
        bool requiresUpdate = false;           ///< Whether to update the world matrix
    };

    /**
    * Constructor
    * @param name The name of the data
    * @param shader The ID of the shader to use
    * @param shaderName The name of the shader to use
    */
    MeshData(const std::string& name, 
             const std::string& shaderName,
             int shaderID);

    /**
    * Destructor
    */
    virtual ~MeshData();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Ticks the mesh
    * @param cameraPosition The world position of the camera
    * @param cameraBounds Bounding area in front of the camera
    * @param causticsTexture The ID of the current texture for caustics
    */
    void Tick(const glm::vec3& cameraPosition, 
              const BoundingArea& cameraBounds,
              int causticsTexture);

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
    * Adds instances at the world center with default values
    */
    void AddInstances(int amount);

    /**
    * Clears all currently created instances
    */
    void ClearInstances();

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

    /**
    * Sets an instance for this mesh
    * @param index The ID of this instance
    * @param position The position offset 
    */
    void SetInstance(int index, const glm::vec3& position);

    /**
    * Gets the instance at the index
    * @param index The index of the instance to get
    * @return the instance
    */
    const Instance& GetInstance(int index) const;

    /**
    * Sets whether this mesh is a sky box
    */
    void SetSkyBox();

protected:

    /**
    * @return whether this mesh renders with caustics
    */
    bool UsesCaustics() const;

    /**
    * @return whether this mesh renders with bump mapping
    */
    bool UsesBumpMapping() const;

    /**
    * @return whether this mesh renders with specular highlights
    */
    bool UsesSpecular() const;

    int m_vertexComponentCount = 0;     ///< Number of components that make up a vertex
    std::vector<float> m_vertices;      ///< Mesh Vertex information
    std::vector<DWORD> m_indices;       ///< Mesh Index information
    std::vector<Instance> m_instances;  ///< Instances of this mesh

private:

    /**
    * Prevent copying
    */
    MeshData(const MeshData&) = delete;
    MeshData& operator=(const MeshData&) = delete;

    /**
    * Determines whether the instance should be rendered
    * @param instance The instance to check
    * @param position The position of the camera
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const Instance& instance,
                      const glm::vec3& position, 
                      const BoundingArea& bounds);

    /**
    * Determines the radius surrounding this mesh
    * This is the based on the furthest vertex from the mesh center
    */
    void GenerateRadius();

    bool m_backfacecull = true;         ///< Whether backface culling is enabled
    int m_instancesRendered = 0;        ///< Number of instances currently rendered
    const std::string m_name;           ///< Name of the mesh
    int m_shaderIndex = -1;             ///< Unique Index of the mesh shader to use
    GLuint m_vaoID = 0;                 ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID = 0;                 ///< Unique ID for the Vertex Buffer Object (VBO)   
    GLuint m_iboID = 0;                 ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;         ///< Whether the vertex buffer object is initialised or not
    std::vector<int> m_textureIDs;      ///< IDs for each texture used
    const std::string m_shaderName;     ///< Name of the shader to use
    bool m_skybox = false;              ///< Whether this mesh is a skybox
    float m_radius = 0.0f;              ///< The radius of the sphere surrounding the mesh
};