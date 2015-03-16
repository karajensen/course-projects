////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"

MeshData::MeshData(const std::string& name, int shader) :
    m_name(name),
    m_shaderIndex(shader)
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

void MeshData::CreateGrid(const glm::vec3& position, float spacing, int rows, int columns)
{
    const int vertices = rows * columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((rows-1)*(columns-1)) * trianglesPerQuad;

    m_vertexComponentCount = 5;
    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    glm::vec3 initialPosition = position;
    initialPosition.x -= spacing * (rows * 0.5f);
    initialPosition.z -= spacing * (columns * 0.5f);

    float u = 0;
    float v = 0;
    int index = 0;

    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < columns; ++c)
        {
            m_vertices[index] = initialPosition.x + (r * spacing);
            m_vertices[index + 1] = initialPosition.y;
            m_vertices[index + 2] = initialPosition.z + (c * spacing);
            m_vertices[index + 3] = u;
            m_vertices[index + 4] = v;

            index += m_vertexComponentCount;
            u += 0.5;
        }
        u = 0;
        v += 0.5;
    }

    index = 0;
    for(int r = 0; r < rows-1; ++r)
    {
        for(int c = 0; c < columns-1; ++c)
        {
            m_indices[index] = r * columns + c;
            m_indices[index + 1] = (r + 1) * columns + c;
            m_indices[index + 2] = (r + 1) * columns + (c + 1);

            m_indices[index + 3] = r * columns + c;
            m_indices[index + 4] = (r + 1) * columns + (c + 1);
            m_indices[index + 5] = r * columns + (c + 1);
        
            index += 6;
        }
    }
}

bool MeshData::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_indices.size(), 
        &m_indices[0], GL_STATIC_DRAW);

    if(HasCallFailed())
    {
        LogError("Failed to create " + m_name + " buffers");
        return false;
    }

    LogInfo("Mesh: " + m_name + " created");
    m_initialised = true;

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