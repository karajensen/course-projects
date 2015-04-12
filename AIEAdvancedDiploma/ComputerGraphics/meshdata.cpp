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
    tweaker.AddEntry("Name", [this](){ return m_name; });
    tweaker.AddEntry("Shader", [this](){ return m_shaderName; });
    tweaker.AddEntry("Instances", [this](){ return std::to_string(m_instances.size()); });
    tweaker.AddEntry("Backface Cull", &m_backfacecull, TW_TYPE_BOOLCPP);
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

    return true;
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

int MeshData::VertexComponentCount() const
{
    return m_vertexComponentCount;
}

void MeshData::SetTextures(int diffuse, int normal, int specular)
{
    m_textureIDs[COLOUR] = diffuse;
    m_textureIDs[NORMAL] = normal;
    m_textureIDs[SPECULAR] = specular;
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
        glm::mat4 scale;
        scale[0][0] = instance.scale.x;
        scale[1][1] = instance.scale.y;
        scale[2][2] = instance.scale.z;

        glm::mat4 translate;
        translate[3][0] = instance.position.x;
        translate[3][1] = instance.position.y;
        translate[3][2] = instance.position.z;
            
        glm::mat4 rotate;
        if (instance.rotation.x == 0 &&
            instance.rotation.y == 0 &&
            instance.rotation.z == 0)
        {
            glm::mat4 rotateX, rotateY, rotateZ;
            glm::rotate(rotateX, instance.rotation.x, glm::vec3(1,0,0));
            glm::rotate(rotateY, instance.rotation.y, glm::vec3(0,1,0));
            glm::rotate(rotateZ, instance.rotation.z, glm::vec3(0,0,1));
            rotate = rotateZ * rotateX * rotateY;
        }

        renderInstance(translate * rotate * scale);
        Render();
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

bool MeshData::SupportsCaustics() const
{
    return m_shaderName.find("caustic") != NO_INDEX;
}

bool MeshData::SupportsBumpMapping() const
{
    return m_shaderName.find("bump") != NO_INDEX;
}

bool MeshData::SupportsSpecular() const
{
    return m_shaderName.find("specular") != NO_INDEX;
}