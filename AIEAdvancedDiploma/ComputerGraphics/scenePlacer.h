////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenePlacer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include <vector>

class Tweaker;
class Terrain;
class Water;
struct SceneData;

/**
* Updates the scene depending on the camera position
* Holds information about the area which consists of patches of tiled water/sand
* Assumes water and sand patches are aligned and the same size
*/
class ScenePlacer
{
public:

    /**
    * Constructor
    * @param data Data for manipulating the scene
    */
    ScenePlacer(SceneData& data);

    /**
    * Destructor
    */
    ~ScenePlacer();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Initialises the scene
    * @param camera The position of the camera
    * @return whether initialisation was successful
    */
    bool Initialise(const glm::vec3& camera);

    /**
    * Updates the scene
    * @param camera The position of the camera
    */
    void Update(const glm::vec3& cameraPosition);

    /**
    * Resets the patches including foliage and emitter placement
    */
    void ResetPatches();

private:

    /**
    * Prevent copying
    */
    ScenePlacer(const ScenePlacer&) = delete;
    ScenePlacer& operator=(const ScenePlacer&) = delete;

    /**
    * Updates the patch to a new position
    * @param row/column The row and column of the patch
    * @param direction The direction the patch is moving
    * @note assumes direction is either [0,1] or [1,0]
    */
    void UpdatePatch(int row,
                     int column,
                     const glm::ivec2& direction);

    /**
    * Determines which patch the positin is inside
    * @param position the position the use
    * @note y component of position is unused
    * @return the patch the position is inside
    */
    glm::ivec2 GetPatchInside(const glm::vec3& position) const;

    /**
    * @return the index from the row/column
    */
    int Index(int row, int column) const;

    /**
    * @return whether the given position is inside the patch
    * @param position the position the use
    * @param row/column The position of the patch to test
    * @note y component of position is unused
    * @return if the position is inside the pathc
    */
    bool IsInsidePatch(const glm::vec3& position, int row, int column) const;

    /**
    * Shifts the patches so the camera is always in the center of the scene
    * @param direction The direction the camera is moving in the patch grid
    * @note assumes direction is either [0,1] or [1,0]
    */
    void ShiftPatches(const glm::ivec2& direction);

    /**
    * Assigns foliage and rocks to all the patches
    */
    void GeneratePatchData();

    /**
    * Places the assigned foliage on the patch
    * @param instance The instance ID to update
    */
    void PlaceFoliage(int instanceID);

    /**
    * Places the assigned emitters on the patch
    * @param instance The instance ID to update
    */
    void PlaceEmitters(int instanceID);

    /**
    * Updates any data stored for the patch
    * @param instance The instance ID to update
    */
    void UpdatePatchData(int instanceID);

    /**
    * Determines the approximate height at the given coordinates
    * @param patchID The ID of the patch to update
    * @param x,z The coordinates to get the height at
    */
    float GetPatchHeight(int instanceID, float x, float z) const;

    /**
    * Key for obtaining the instance assigned to a patch
    */
    struct InstanceKey
    {
        int index = 0;       ///< Scene data ID
        int instance = 0;    ///< Instance ID
    };

    /**
    * Holds information on what meshes exist inside the patch
    */
    struct Patch
    {
        glm::vec2 minBounds;                ///< Maximum global coordinates of the patch 
        glm::vec2 maxBounds;                ///< Minimum global coordinates of the patch
        std::vector<InstanceKey> foliage;   ///< Data for what foliage to use
        std::vector<InstanceKey> emitters;  ///< Data for what emitters to use
    };

    SceneData& m_data;              ///< Data for manipulating the scene
    Terrain& m_sand;                ///< Main Sand terrain mesh
    Water& m_ocean;                 ///< Main Ocean terran mesh
    int m_patchPerRow = 0;          ///< The number of patches per row of the area
    int m_countRandom = 5;          ///< Instance count to vary for foliage
    float m_patchSize = 0.0f;       ///< The offset between sand/water patches
    std::vector<int> m_patches;     ///< The current ordering of the patches; holds the instance ID
    std::vector<int> m_previous;    ///< Buffer for reorganising the patches; holds the instance ID
    std::vector<Patch> m_patchData; ///< Holds patch data; key is the instance ID held in m_patches
    glm::ivec2 m_patchInside;       ///< The patch the camera is currently inside
};