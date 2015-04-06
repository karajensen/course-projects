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
    */
    void Initialise(const glm::vec3& camera);

private:

    /**
    * Prevent copying
    */
    SceneUpdater(const SceneUpdater&) = delete;
    SceneUpdater& operator=(const SceneUpdater&) = delete;

    /**
    * @return the index from the row/column
    */
    int Index(int row, int column);

    SceneData& m_data;          ///< Data for manipulating the scene
    int m_patchPerRow = 0;      ///< The number of patches per row of the area
    std::vector<int> m_patches; ///< The indices of the sand/water for each area patch
    float m_sandOffset = 0.0f;  ///< The offset between sand patches
    float m_waterOffset = 0.0f; ///< The offset between water patches
};                     