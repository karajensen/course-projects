////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"
#include "tweaker.h"

Mesh::Mesh(const std::string& name, 
           const std::string& shaderName,
           int shaderID) :

    MeshData(name, shaderName, shaderID)
{
}

void Mesh::AddToTweaker(Tweaker& tweaker)
{
    MeshData::AddToTweaker(tweaker);

    if (UsesBumpMapping())
    {
        tweaker.AddFltEntry("Bump Amount", &m_bump, 0.1f);
    }

    if (UsesCaustics())
    {
        tweaker.AddFltEntry("Caustics Amount", &m_causticsAmount, 0.1f);
        tweaker.AddFltEntry("Caustics Scale", &m_causticsScale, 0.1f);
    }

    if (UsesSpecular())
    {
        tweaker.AddFltEntry("Specularity", &m_specularity, 0.1f);
    }    

    tweaker.AddFltEntry("Specular", &m_specular, 0.1f);
    tweaker.AddFltEntry("Ambience", &m_ambience, 0.1f);
    tweaker.AddFltEntry("Diffuse", &m_diffuse, 0.1f);
}

bool Mesh::InitialiseFromFile(const std::string& path, 
                              const glm::vec2& uvScale,
                              bool requiresNormals, 
                              bool requiresTangents)
{
    if (requiresTangents && !requiresNormals)
    {
        LogError(path + " requies normals for bump mapping");
        return false;
    }

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
        if(!pMesh->HasTextureCoords(0))
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
            m_vertices.push_back(pMesh->mTextureCoords[0][vert].x * uvScale.x);
            m_vertices.push_back(pMesh->mTextureCoords[0][vert].y * uvScale.y);
            componentCount = 5;

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

    LogInfo("Mesh: " + Name() + " created");
    return MeshData::Initialise();
}