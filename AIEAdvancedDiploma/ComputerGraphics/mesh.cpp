////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "fbx/FBXFile.h"
#include "mesh.h"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

Mesh::Mesh(const std::string& name, int shader, int instances) :
    MeshData(name, shader)
{
    m_instances.resize(instances);
}

bool Mesh::InitialiseFromFile(const std::string& path, 
                              bool requiresUVs,
                              bool requiresNormals, 
                              bool requiresTangents)
{
    if (requiresTangents && !requiresNormals)
    {
        LogError(path + " requies normals for bump mapping");
        return false;
    }

    bool success = false;

    if (path.find(".obj") != NO_INDEX)
    {
        success = InitialiseFromOBJ(path, requiresUVs, requiresNormals, requiresTangents);
    }
    else if (path.find(".fbx") != NO_INDEX)
    {
        success = InitialiseFromFBX(path, requiresUVs, requiresNormals, requiresTangents);
    }

    return success && MeshData::Initialise();
}


void Mesh::Render(RenderInstance renderInstance) const
{
    for (const Instance& instance : Instances())
    {
        if (instance.shouldRender)
        {
            glm::mat4 scale;
            scale[0][0] = instance.scale;
            scale[1][1] = instance.scale;
            scale[2][2] = instance.scale;

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
            MeshData::Render();
        }
    }
}

const std::vector<Mesh::Instance>& Mesh::Instances() const
{
    return m_instances;
}

 std::vector<Mesh::Instance>& Mesh::Instances()
{
    return m_instances;
}

void Mesh::SetInstance(int index,
                       const glm::vec3& position,
                       const glm::vec3& rotation,
                       float scale)
{
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale = scale;
}

float Mesh::Bump() const
{
    return m_bump;
}

float Mesh::Specularity() const
{
    return m_specularity;
}

float Mesh::Ambience() const
{
    return m_ambience;
}

float Mesh::Caustics() const
{
    return m_caustics;
}

void Mesh::Specularity(float value)
{
    m_specularity = value;
}

void Mesh::Bump(float value)
{
    m_bump = value;
}

void Mesh::Ambience(float value)
{
    m_ambience = value;
}

void Mesh::Caustics(float value)
{
    m_caustics = value;
}

void Mesh::AddInstance()
{
    m_instances.emplace_back();
}

void Mesh::AddInstance(const glm::vec3& position,
                       const glm::vec3& rotation,
                       float scale)

{
    const int index = m_instances.size();
    m_instances.emplace_back();
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale = scale;
}

bool Mesh::InitialiseFromOBJ(const std::string& path,
                             bool requiresUVs,
                             bool requiresNormals,
                             bool requiresTangents)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace|
        aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|
        aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices|aiProcess_GenSmoothNormals|
        aiProcess_LimitBoneWeights|aiProcess_RemoveRedundantMaterials|aiProcess_OptimizeMeshes);

    if(!scene)
    {
        LogError("Assimp import error for mesh " + path + ": " + importer.GetErrorString());
        return false;
    }

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    // For each submesh
    m_vertexComponentCount = 1;
    bool generatedComponentCount = false;
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        const unsigned int indexOffset = m_vertices.size() / m_vertexComponentCount;

        if(!pMesh->HasPositions())
        {
            LogError(Name() + " requires positions for requested shader");
            return false;
        }
        if(requiresUVs && !pMesh->HasTextureCoords(0))
        {
            LogError(Name() + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            LogError(Name() + " requires normals for requested shader");
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            m_vertices.push_back(pMesh->mVertices[vert].x);
            m_vertices.push_back(pMesh->mVertices[vert].y);
            m_vertices.push_back(pMesh->mVertices[vert].z);
            componentCount = 3;

            if (requiresUVs)
            {
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].x);
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].y);
                componentCount += 2;
            }

            if (requiresNormals)
            {
                m_vertices.push_back(pMesh->mNormals[vert].x);
                m_vertices.push_back(pMesh->mNormals[vert].y);
                m_vertices.push_back(pMesh->mNormals[vert].z);
                componentCount += 3;
            }

            if(requiresTangents)
            {
                if(pMesh->HasTangentsAndBitangents())
                {
                    m_vertices.push_back(pMesh->mTangents[vert].x);
                    m_vertices.push_back(pMesh->mTangents[vert].y);
                    m_vertices.push_back(pMesh->mTangents[vert].z);
                    m_vertices.push_back(pMesh->mBitangents[vert].x);
                    m_vertices.push_back(pMesh->mBitangents[vert].y);
                    m_vertices.push_back(pMesh->mBitangents[vert].z);
                    componentCount += 6;
                }
                else
                {
                    LogError(Name() + " requires tangents for requested shader");
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != m_vertexComponentCount)
            {
                LogError("Assimp error for mesh " + path + ": " + 
                    std::to_string(componentCount) + " does not match " +
                    std::to_string(m_vertexComponentCount));
                return false;
            }
        }
        else
        {
            m_vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // For each face
        for(unsigned int face = 0; face < pMesh->mNumFaces; ++face)
        {
            aiFace *pFace = &pMesh->mFaces[face];
            if(pFace->mNumIndices != 3)
            {
                LogError("Assimp error for mesh " + path + ": not all faces are triangles");
                return false;
            }

            m_indices.push_back(indexOffset + pFace->mIndices[0]);
            m_indices.push_back(indexOffset + pFace->mIndices[1]);
            m_indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }
    return true;
}

bool Mesh::InitialiseFromFBX(const std::string& path,
                             bool requiresUVs,
                             bool requiresNormals,
                             bool requiresTangents)
{
    auto fbx = std::make_unique<FBXFile>();

    if (!fbx->load(path.c_str(), FBXFile::UNITS_METER, false, false, false))
    {
        LogError("FBX loader failed for " + m_name);
        return false;
    }

    // For each submesh
    bool generatedComponentCount = false;
    for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
    {
        FBXMeshNode* mesh = fbx->getMeshByIndex(i);

        const unsigned int indexOffset = m_vertices.size() / 
            std::max(1, m_vertexComponentCount);

        // For each vertex
        int componentCount = 0;
        for (unsigned int i = 0; i < mesh->m_vertices.size(); ++i)
        {
            const auto& vertex = mesh->m_vertices[i];
            m_vertices.push_back(vertex.position.x);
            m_vertices.push_back(vertex.position.y);
            m_vertices.push_back(vertex.position.z);
            componentCount = 3;

            if (requiresUVs)
            {
                m_vertices.push_back(vertex.texCoord1.x);
                m_vertices.push_back(vertex.texCoord1.y);
                componentCount += 2;
            }

            if (requiresNormals)
            {
                if (!IsZero(vertex.normal))
                {
                    m_vertices.push_back(vertex.normal.x);
                    m_vertices.push_back(vertex.normal.y);
                    m_vertices.push_back(vertex.normal.z);
                    componentCount += 3;
                }
                else
                {
                    LogError(Name() + " requires normals for requested shader");
                    return false;
                }
            }

            if(requiresTangents)
            {
                if (!IsZero(vertex.tangent) && !IsZero(vertex.binormal))
                {
                    m_vertices.push_back(vertex.tangent.x);
                    m_vertices.push_back(vertex.tangent.y);
                    m_vertices.push_back(vertex.tangent.z);
                    m_vertices.push_back(vertex.binormal.x);
                    m_vertices.push_back(vertex.binormal.y);
                    m_vertices.push_back(vertex.binormal.z);
                    componentCount += 6;
                }
                else
                {
                    LogError(Name() + " requires tangents for requested shader");
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != m_vertexComponentCount)
            {
                LogError("FBX error for mesh " + path + ": " + 
                    std::to_string(componentCount) + " does not match " +
                    std::to_string(m_vertexComponentCount));
                return false;
            }
        }
        else
        {
            m_vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // Save the indices
        for (unsigned int i = 0; i < mesh->m_indices.size(); ++i)
        {
            m_indices.push_back(indexOffset + mesh->m_indices[i]);
        }
    }

    fbx->unload();
    return true;
}