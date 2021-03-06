////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneData.h
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "shader.h"
#include "mesh.h"
#include "water.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "texture.h"
#include "animation.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "meshgroup.h"
#include "quad.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    /**
    * Constructor
    */
    SceneData() :
        post(std::make_unique<PostProcessing>())
    {
    }

    std::vector<std::unique_ptr<Shader>> shaders;      ///< All shaders in the scene
    std::vector<std::unique_ptr<Mesh>> meshes;         ///< All meshes in the scene
    std::vector<std::unique_ptr<Light>> lights;        ///< All lights in the scene
    std::vector<std::unique_ptr<Texture>> textures;    ///< All textures in the scene
    std::vector<std::unique_ptr<Terrain>> terrain;     ///< All terrain in the scene
    std::vector<std::unique_ptr<Water>> water;         ///< All water in the scene
    std::vector<std::unique_ptr<Emitter>> emitters;    ///< All emitters in the scene
    std::vector<std::unique_ptr<Animation>> animation; ///< Managers for animated textures
    std::unique_ptr<Quad> shadows;                     ///< Shadow decals for terrain
    std::unique_ptr<PostProcessing> post;              ///< Data for post processing
    std::vector<unsigned int> proceduralTextures;      ///< Indexes for procedural textures
    std::vector<MeshGroup> foliage;                    ///< Available foliage for placing in scene
    std::vector<InstanceKey> rocks;                    ///< Avaliable rocks for placing in scene
    unsigned int sandIndex = 0;                        ///< Index for the sand terrain
    unsigned int oceanIndex = 0;                       ///< Index for the ocean grid
};