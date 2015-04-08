////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneUpdater.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include <vector>

struct SceneData;

/**
* Updates the scene depending on the camera position
* Holds information about the area which consists of patches of tiled water/sand
* Assumes water and sand patches are aligned and the same size
*/
class SceneUpdater
{
public:

    /**
    * Constructor
    * @param data Data for manipulating the scene
    */
    SceneUpdater(SceneData& data);

    /**
    * Destructor
    */
    ~SceneUpdater();

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
    void Update(const glm::vec3& camera);

private:

    /**
    * Prevent copying
    */
    SceneUpdater(const SceneUpdater&) = delete;
    SceneUpdater& operator=(const SceneUpdater&) = delete;

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

    SceneData& m_data;           ///< Data for manipulating the scene
    int m_patchPerRow = 0;       ///< The number of patches per row of the area
    std::vector<int> m_patches;  ///< The indices of the sand/water for each area patch
    std::vector<int> m_scratch;  ///< Buffer for reorganising the patches
    glm::ivec2 m_patchInside;    ///< The patch the camera is currently inside
    float m_patchSize = 0.0f;    ///< The offset between sand/water patches
};                     