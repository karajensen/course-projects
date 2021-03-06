////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "grid.h"
#include "common.h"
#include "tweaker.h"

namespace
{
    enum VertexOffset
    {
        POS_X,
        POS_Y,
        POS_Z,
        TEXTURE_U,
        TEXTURE_V,
        NORMAL_X, 
        NORMAL_Y, 
        NORMAL_Z,
        TANGENT_X,
        TANGENT_Y,
        TANGENT_Z,
        BITANGENT_X,
        BITANGENT_Y,
        BITANGENT_Z
    };

    /**
    * Utility function for multiplying a vector
    */
    glm::vec3 operator*(const glm::vec3& v, double x)
    {
        return v * glm::vec3(x, x, x);
    }

    /**
    * Utility function for dividing a vector
    */
    glm::vec3 operator/(const glm::vec3& v, double x)
    {
        return v / glm::vec3(x, x, x);
    }
}

Grid::Grid(const std::string& name, const std::string& shaderName, int shaderID) :
    MeshData(name, shaderName, shaderID)
{
}

void Grid::AddToTweaker(Tweaker& tweaker)
{
    MeshData::AddToTweaker(tweaker);
}

bool Grid::CreateGrid(const glm::vec2& uvStretch,
                      float spacing, 
                      int rows, 
                      int columns, 
                      bool normals, 
                      bool tangents)
{
    if (!normals && tangents)
    {
        LogError("CreateGrid: Tangents require normals");
        return false;
    }

    m_columns = columns;
    m_rows = rows;
    m_spacing = spacing;
    m_vertexComponentCount = tangents ? 14 : (normals ? 8 : 5);
    m_hasNormals = normals;
    m_hasTangents = tangents;
    m_uvStretch = uvStretch;

    ResetGrid();
    return true;
}

void Grid::ResetGrid()
{
    const int vertices = m_rows * m_columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((m_rows-1)*(m_columns-1)) * trianglesPerQuad;

    const float uOffset = m_uvStretch.x / static_cast<float>(m_rows-1);
    const float vOffset = m_uvStretch.y / static_cast<float>(m_columns-1);

    m_indices.clear();
    m_vertices.clear();

    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    glm::vec3 initialPosition;
    initialPosition.x -= m_spacing * ((m_rows-1) * 0.5f);
    initialPosition.z -= m_spacing * ((m_columns-1) * 0.5f);

    float u = 0;
    float v = 0;
    int index = 0;

    for(int r = 0; r < m_rows; ++r)
    {
        for(int c = 0; c < m_columns; ++c)
        {
            m_vertices[index + POS_X] = initialPosition.x + (r * m_spacing);
            m_vertices[index + POS_Y] = initialPosition.y;
            m_vertices[index + POS_Z] = initialPosition.z + (c * m_spacing);
            m_vertices[index + TEXTURE_U] = u;
            m_vertices[index + TEXTURE_V] = v;

            if (m_hasNormals)
            {
                m_vertices[index + NORMAL_X] = 0.0f;
                m_vertices[index + NORMAL_Y] = 1.0f;
                m_vertices[index + NORMAL_Z] = 0.0f;
            }

            if (m_hasTangents)
            {
                m_vertices[index + TANGENT_X] = 1.0f;
                m_vertices[index + TANGENT_Y] = 0.0f;
                m_vertices[index + TANGENT_Z] = 0.0f;
                m_vertices[index + BITANGENT_X] = 0.0f;
                m_vertices[index + BITANGENT_Y] = 0.0f;
                m_vertices[index + BITANGENT_Z] = 1.0f;
            }

            index += m_vertexComponentCount;
            u += uOffset;
        }

        u = 0;
        v += vOffset;
    }

    index = 0;
    for(int r = 0; r < m_rows-1; ++r)
    {
        for(int c = 0; c < m_columns-1; ++c)
        {
            m_indices[index] = r * m_columns + c;
            m_indices[index + 1] = (r + 1) * m_columns + (c + 1);
            m_indices[index + 2] = (r + 1) * m_columns + c;

            m_indices[index + 3] = r * m_columns + c;
            m_indices[index + 4] = r * m_columns + (c + 1);
            m_indices[index + 5] = (r + 1) * m_columns + (c + 1);
        
            index += 6;
        }
    }
}

float Grid::Spacing() const
{
    return m_spacing;
}

unsigned int Grid::GetIndex(int row, int column) const
{
    return (row * m_columns * m_vertexComponentCount)
        + (column * m_vertexComponentCount);
}

float Grid::GetHeight(int row, int column) const
{
    return m_vertices[GetIndex(row, column) + POS_Y];
}

void Grid::SetHeight(int row, int column, float height)
{
    m_vertices[GetIndex(row, column) + POS_Y] = height;
}

int Grid::Rows() const
{
    return m_rows;
}

int Grid::Columns() const
{
    return m_columns;
}

glm::vec3 Grid::GetPosition(int row, int column) const
{
    return GetPosition(GetIndex(row, column));
}

glm::vec3 Grid::GetPosition(int index) const
{
    return glm::vec3(m_vertices[index + POS_X],
        m_vertices[index + POS_Y], m_vertices[index + POS_Z]);
}

glm::vec3 Grid::GetNormal(int index) const
{
    return glm::vec3(m_vertices[index + NORMAL_X],
        m_vertices[index + NORMAL_Y], m_vertices[index + NORMAL_Z]);
}

glm::vec3 Grid::GetTangent(int index) const
{
    return glm::vec3(m_vertices[index + TANGENT_X],
        m_vertices[index + TANGENT_Y], m_vertices[index + TANGENT_Z]);
}

glm::vec2 Grid::GetUVs(int index) const
{
    return glm::vec2(m_vertices[index + TEXTURE_U],
        m_vertices[index + TEXTURE_V]);
}

float Grid::Size() const
{
    assert(Rows() == Columns());
    return m_spacing * (Rows()-1);
}

void Grid::RecalculateNormals()
{
    if (!m_hasNormals)
    {
        return;
    }

    // For each triangle add the normal to the vertex
    for (int r = 0; r < m_rows - 1; ++r)
    {
        for (int c = 0; c < m_columns - 1; ++c)
        {
            const int p1index = GetIndex(r, c);
            const int p2index = GetIndex(r + 1, c);
            const int p3index = GetIndex(r, c + 1);
            const int p4index = GetIndex(r + 1, c + 1);

            const glm::vec3 p1(GetPosition(p1index));
            const glm::vec3 p2(GetPosition(p2index));
            const glm::vec3 p3(GetPosition(p3index));
            const glm::vec3 p4(GetPosition(p4index));

            glm::vec3 normal = glm::cross(p1 - p2, p3 - p2);
            normal = glm::normalize(normal);

            m_vertices[p1index + NORMAL_X] += normal.x;
            m_vertices[p1index + NORMAL_Y] += normal.y;
            m_vertices[p1index + NORMAL_Z] += normal.z;

            m_vertices[p2index + NORMAL_X] += normal.x;
            m_vertices[p2index + NORMAL_Y] += normal.y;
            m_vertices[p2index + NORMAL_Z] += normal.z;

            m_vertices[p3index + NORMAL_X] += normal.x;
            m_vertices[p3index + NORMAL_Y] += normal.y;
            m_vertices[p3index + NORMAL_Z] += normal.z;

            normal = glm::cross(p2 - p4, p3 - p4);
            normal = glm::normalize(normal);

            m_vertices[p2index + NORMAL_X] += normal.x;
            m_vertices[p2index + NORMAL_Y] += normal.y;
            m_vertices[p2index + NORMAL_Z] += normal.z;

            m_vertices[p3index + NORMAL_X] += normal.x;
            m_vertices[p3index + NORMAL_Y] += normal.y;
            m_vertices[p3index + NORMAL_Z] += normal.z;

            m_vertices[p4index + NORMAL_X] += normal.x;
            m_vertices[p4index + NORMAL_Y] += normal.y;
            m_vertices[p4index + NORMAL_Z] += normal.z;
        }
    }

    // For each triangle add the tangent to the vertex
    if (m_hasTangents)
    {
        for (int r = 0; r < m_rows - 1; ++r)
        {
            for (int c = 0; c < m_columns - 1; ++c)
            {
                const int p0index = GetIndex(r, c);
                const int p1index = GetIndex(r + 1, c);
                const int p2index = GetIndex(r, c + 1);

                const glm::vec3 p0(GetPosition(p0index));
                const glm::vec3 p1(GetPosition(p1index));
                const glm::vec3 p2(GetPosition(p2index));

                const glm::vec2 uv0(GetUVs(p0index));
                const glm::vec2 uv1(GetUVs(p1index));
                const glm::vec2 uv2(GetUVs(p2index));

                const glm::vec3 q1(p1 - p0);
                const glm::vec3 q2(p2 - p0);
                const double s1 = uv1.x - uv0.x;
                const double s2 = uv2.x - uv0.x;
                const double t1 = uv1.y - uv0.y;
                const double t2 = uv2.y - uv0.y;

                // Mathematics for 3D Game Programming and Computer Graphics p184
                // Plane Equation is p - p0 = sT + tB
                // Solve T using q1 = s1T + t1B and q2 = s2T + t2B
                // Substitute B = (q1 - s1T) / t1 into q2 = s2T + t2B
                // Rearranging T = (q2 - t2q1/t1) / (s2 - (t2s1/t1))
                const glm::vec3 tangent = (q2 - ((q1 * t2) / t1)) / (s2 - ((t2 * s1) / t1));

                m_vertices[p0index + TANGENT_X] += tangent.x;
                m_vertices[p0index + TANGENT_Y] += tangent.y;
                m_vertices[p0index + TANGENT_Z] += tangent.z;

                m_vertices[p1index + TANGENT_X] += tangent.x;
                m_vertices[p1index + TANGENT_Y] += tangent.y;
                m_vertices[p1index + TANGENT_Z] += tangent.z;

                m_vertices[p2index + TANGENT_X] += tangent.x;
                m_vertices[p2index + TANGENT_Y] += tangent.y;
                m_vertices[p2index + TANGENT_Z] += tangent.z;
            }
        }

        for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
        {
            const glm::vec3 normal = glm::normalize(GetNormal(vertex));
            m_vertices[vertex + NORMAL_X] = normal.x;
            m_vertices[vertex + NORMAL_Y] = normal.y;
            m_vertices[vertex + NORMAL_Z] = normal.z;                

            if (m_hasTangents)
            {
                const glm::vec3 tangent = glm::normalize(GetTangent(vertex));
                m_vertices[vertex + TANGENT_X] = tangent.x;
                m_vertices[vertex + TANGENT_Y] = tangent.y;
                m_vertices[vertex + TANGENT_Z] = tangent.z;                

                // Bitangent is orthogonal to the normal/tangent
                const glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));
                m_vertices[vertex + BITANGENT_X] = bitangent.x;
                m_vertices[vertex + BITANGENT_Y] = bitangent.y;
                m_vertices[vertex + BITANGENT_Z] = bitangent.z;
            }
        }
    }
}

const glm::vec2& Grid::GetUVStretch() const
{
    return m_uvStretch;
}

void Grid::SetUVStretch(const glm::vec2& value)
{
    m_uvStretch = value;
}