////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneUpdater.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneUpdater.h"
#include "sceneData.h"

namespace
{
    const int WATER_ID = 0;
}

SceneUpdater::SceneUpdater(SceneData& data) :
    m_data(data)
{
    const int patchAmount = 9;
    double unused = 0.0;
    if (std::modf(std::sqrt(static_cast<double>(patchAmount)), &unused) != 0.0)
    {
        LogError("Area patch size unusable");
    }

    m_patches.resize(patchAmount);
    m_scratch.resize(patchAmount);
    m_patchPerRow = static_cast<int>(
        std::sqrt(static_cast<double>(patchAmount)));
}

SceneUpdater::~SceneUpdater() = default;

int SceneUpdater::Index(int row, int column) const
{
    return row * m_patchPerRow + column;
}

bool SceneUpdater::IsInsidePatch(const glm::vec3& position, int row, int column) const
{
    const float halfSize = m_patchSize * 0.5f;
    const int instanceID = m_patches[Index(row, column)];
    const auto& instance = m_data.water[WATER_ID]->Instances()[instanceID];
        
    return position.x < instance.position.x + halfSize &&
        position.z < instance.position.z + halfSize &&
        position.x >= instance.position.x - halfSize &&
        position.z >= instance.position.z - halfSize;
}

glm::ivec2 SceneUpdater::GetPatchInside(const glm::vec3& position) const
{
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            if (IsInsidePatch(position, r, c))
            {
                return glm::ivec2(r, c);
            }
        }
    }
    return glm::ivec2(NO_INDEX, NO_INDEX);
}

void SceneUpdater::Update(const glm::vec3& camera)
{
    if (m_patchInside.x == NO_INDEX ||
        m_patchInside.y == NO_INDEX ||
        IsInsidePatch(camera, m_patchInside.x, m_patchInside.y))
    {
        return;
    }

    const glm::ivec2 previousPatch = m_patchInside;
    m_patchInside = GetPatchInside(camera);
    const glm::ivec2 direction = previousPatch - m_patchInside;

    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[WATER_ID];

    // Shift the patches so the camera is always in the center
    const int maxIndex = m_patchPerRow - 1;
    m_scratch = m_patches;

    if (direction.x > 0)
    {
        // Shift the bottom row to the top
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            m_patches[Index(0, c)] = m_scratch[Index(maxIndex, c)];
        }

        // Move all other rows down
        for (int r = 0; r < m_patchPerRow-1; ++r)
        {
            for (int c = 0; c < m_patchPerRow; ++c)
            {
                m_patches[Index(r+1, c)] = m_scratch[Index(r, c)];
            }
        }

        // Update the new position of the new top row
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const int patchID = m_patches[Index(0, c)];
            const int nextID = m_patches[Index(1, c)];
            const auto& instance = water.GetInstance(nextID);
            const glm::vec2 position(instance.position.x - m_patchSize, instance.position.z);
            water.SetInstance(patchID, position, false, false);
            sand.SetInstance(patchID, position);
            //sand.Instances()[patchID].position.y = 0.0f;
        }
    }
    else if (direction.x < 0)
    {
        // Shift the top row to the bottom
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            m_patches[Index(maxIndex, c)] = m_scratch[Index(0, c)];
        }

        // Move all other rows up
        for (int r = maxIndex; r > 0; --r)
        {
            for (int c = 0; c < m_patchPerRow; ++c)
            {
                m_patches[Index(r-1, c)] = m_scratch[Index(r, c)];
            }
        }

        // Update the new position of the new top row
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const int patchID = m_patches[Index(maxIndex, c)];
            const int nextID = m_patches[Index(maxIndex-1, c)];
            const auto& instance = water.GetInstance(nextID);
            const glm::vec2 position(instance.position.x + m_patchSize, instance.position.z);
            water.SetInstance(patchID, position, false, false);
            sand.SetInstance(patchID, position);
            //sand.Instances()[patchID].position.y = 0.0f;
        }
    }

    if (direction.x > 0)
    {
        // Shift the left row to the right and shuffle left

    }
    else if (direction.x < 0) 
    {
        // Shift the right row to the left and shuffle right

    }

    // Patches have changed, re-find the correct value
    m_patchInside = GetPatchInside(camera);
}

bool SceneUpdater::Initialise(const glm::vec3& camera)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    assert(!m_data.water.empty());
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[WATER_ID];

    const float sandSize = sand.Size();
    const float waterSize = water.Size();
    assert(sandSize == waterSize);
    m_patchSize = sandSize;

    const float offset = (halfPatch * m_patchSize) - (m_patchSize / 2.0f);
    const glm::vec2 start(camera.x - offset, camera.z - offset);

    int instance = 0;
    glm::vec2 position;

    // Create the tiling water/sand instances
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const bool xFlipped = r % 2 == 0;
            const bool zFlipped = c % 2 == 0;

            position.x = start.x + (r * m_patchSize);
            position.y = start.y + (c * m_patchSize);

            water.AddInstance(position, false, false);
            sand.AddInstance(position);

            sand.Instances()[(Index(r,c))].scale *= 0.98f;

            m_patches[instance] = instance;
            ++instance;
        }
    }

    // Fill the patches so the current patch is not occluded by terrain
    m_patchInside = GetPatchInside(camera);
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        LogError("Did not start inside recognised patch");
    }

    return true;   
}