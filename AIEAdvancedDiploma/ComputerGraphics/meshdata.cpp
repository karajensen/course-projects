////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"
#include "tweaker.h"

MeshData::MeshData(const std::string& name, 
                   const std::string& shaderName,
                   int shaderID) :
    m_name(name),
    m_shaderName(shaderName),
    m_shaderIndex(shaderID)
{
    m_textureIDs.resize(MAX_TYPES);
    m_textureIDs.assign(MAX_TYPES, NO_INDEX);    
}

MeshData::~MeshData()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

void MeshData::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", [this](){ return m_name; });
    tweaker.AddStrEntry("Shader", [this](){ return m_shaderName; });
    tweaker.AddIntEntry("Instances", [this](){ return m_instances.size(); });
    tweaker.AddEntry("Visible", &m_instancesRendered, TW_TYPE_INT32, true);
    tweaker.AddEntry("Backface Cull", &m_backfacecull, TW_TYPE_BOOLCPP);
    tweaker.AddFltEntry("Radius", &m_radius);
}

bool MeshData::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(1, &m_vboID);
    glGenBuffers(1, &m_iboID);
    m_initialised = true;

    if (Reload())
    {
        LogInfo("Mesh: " + m_name + " created");
        return true;
    }
    return false;
}

bool MeshData::Reload()
{
    glBindVertexArray(m_vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_indices.size(), 
        &m_indices[0], GL_STATIC_DRAW);

    if(HasCallFailed())
    {
        LogError("Failed to set " + m_name + " buffers");
        return false;
    }

    GenerateRadius();

    return true;
}

void MeshData::GenerateRadius()
{
    m_radius = 0.0f;

    // Assumes position is always first in a vertex
    for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
    {
        const glm::vec3 position(m_vertices[vertex], 
            m_vertices[vertex + 1], m_vertices[vertex + 2]);
        m_radius = std::max(m_radius, glm::length(position));
    }
}

void MeshData::PreRender() const
{
    assert(m_initialised);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void MeshData::Render() const
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

const std::string& MeshData::Name() const
{
    return m_name;
}

int MeshData::ShaderID() const
{
    return m_shaderIndex;
}

const std::vector<float>& MeshData::Vertices() const
{
    return m_vertices;
}

const std::vector<unsigned long>& MeshData::Indices() const
{
    return m_indices;
}

const std::vector<int>& MeshData::TextureIDs() const
{
    return m_textureIDs;
}

void MeshData::SetTexture(Slot type, int ID)
{
    if (ID == NO_INDEX)
    {
        LogError("Texture ID invalid");
    }
    m_textureIDs[type] = ID;
}

bool MeshData::BackfaceCull() const
{
    return m_backfacecull;
}

void MeshData::BackfaceCull(bool value)
{
    m_backfacecull = value;
}

void MeshData::Render(RenderInstance renderInstance) const
{
    for (const Instance& instance : Instances())
    {
        if (instance.render)
        {
            renderInstance(instance.world);
            Render();
        }
    }
}

const std::vector<MeshData::Instance>& MeshData::Instances() const
{
    return m_instances;
}

 std::vector<MeshData::Instance>& MeshData::Instances()
{
    return m_instances;
}

void MeshData::SetInstance(int index, const glm::vec3& position)
{
    m_instances[index].position = position;
    m_instances[index].requiresUpdate = true;
}

void MeshData::SetInstance(int index,
                           const glm::vec3& position,
                           const glm::vec3& rotation,
                           float scale)
{
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale.x = scale;
    m_instances[index].scale.y = scale;
    m_instances[index].scale.z = scale;
    m_instances[index].requiresUpdate = true;
}

void MeshData::AddInstance()
{
    m_instances.emplace_back();
}

void MeshData::AddInstance(const glm::vec3& position,
                           const glm::vec3& rotation,
                           float scale)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), 
        position, rotation, scale);
}

const MeshData::Instance& MeshData::GetInstance(int index) const
{
    return m_instances.at(index);
}

bool MeshData::UsesCaustics() const
{
    return m_textureIDs[CAUSTICS] != NO_INDEX;   
}

bool MeshData::UsesBumpMapping() const
{
    return m_textureIDs[NORMAL] != NO_INDEX;   
}

bool MeshData::UsesSpecular() const
{
    return m_textureIDs[SPECULAR] != NO_INDEX;   
}

bool MeshData::ShouldRender(const Instance& instance,
                            const glm::vec3& position, 
                            const BoundingArea& bounds)
{
    const float scale = std::max(std::max(instance.scale.x, instance.scale.y), instance.scale.z);
    const glm::vec3 centerToMesh = instance.position - bounds.center;
    return glm::length(centerToMesh) <= (m_radius * scale) + bounds.radius;
}

void MeshData::SetSkyBox()
{
    m_skybox = true;
}

void MeshData::Tick(const glm::vec3& cameraPosition, 
                    const BoundingArea& cameraBounds,
                    int causticsTexture)
{
    if (UsesCaustics())
    {
        SetTexture(CAUSTICS, causticsTexture);
    }
   
    m_instancesRendered = 0;
    for (auto& instance : m_instances)
    {
        instance.render = ShouldRender(
            instance, cameraPosition, cameraBounds);

        if (m_skybox)
        {
            instance.position = cameraPosition;
            instance.requiresUpdate = true;
        }

        if (instance.render)
        {
            ++m_instancesRendered;
        }

        if (instance.requiresUpdate)
        {
            instance.requiresUpdate = false;

            if (instance.rotation.x == 0 &&
                instance.rotation.y == 0 &&
                instance.rotation.z == 0)
            {
                glm::mat4 scale;
                scale[0][0] = instance.scale.x;
                scale[1][1] = instance.scale.y;
                scale[2][2] = instance.scale.z;

                glm::mat4 translate;
                translate[3][0] = instance.position.x;
                translate[3][1] = instance.position.y;
                translate[3][2] = instance.position.z;

                glm::mat4 rotateX, rotateY, rotateZ;
                glm::rotate(rotateX, instance.rotation.x, glm::vec3(1, 0, 0));
                glm::rotate(rotateY, instance.rotation.y, glm::vec3(0, 1, 0));
                glm::rotate(rotateZ, instance.rotation.z, glm::vec3(0, 0, 1));

                instance.world = translate * (rotateZ * rotateX * rotateY) * scale;
            }
            else
            {
                instance.world[0][0] = instance.scale.x;
                instance.world[1][1] = instance.scale.y;
                instance.world[2][2] = instance.scale.z;
                instance.world[3][0] = instance.position.x;
                instance.world[3][1] = instance.position.y;
                instance.world[3][2] = instance.position.z;
            }
        }
    }
}