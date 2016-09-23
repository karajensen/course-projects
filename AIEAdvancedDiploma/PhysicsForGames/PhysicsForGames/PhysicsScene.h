////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "glm/vec2.hpp"

namespace aie
{
    class Renderer2D;
}

class PhysicsObject;

class PhysicsScene
{
public:

    /**
    * Constructor
    */
    PhysicsScene();

    /**
    * Destructor
    */
    ~PhysicsScene();

    /**
    * Adds a new actor to the scene
    */
    void AddActor(std::unique_ptr<PhysicsObject> actor);

    /**
    * Updates the scene
    */
    void Update();

    /**
    * Outputs debug information about the scene and actors
    */
    void DebugScene();

    /**
    * Renders the scene
    * @param renderer The 2D renderer for drawing the scene
    */
    void Draw(aie::Renderer2D* renderer);

    /**
    * Sets the gravity force
    * @param x/y The components of the gravity
    */
    void SetGravity(float x, float y);

    /**
    * Sets the gravity force
    * @param gravity The gravity to set
    */
    void SetGravity(const glm::vec2& gravity);

    /**
    * Sets the time to pass between updates on actors
    */
    void SetTimeStep(float timeStep);

    /**
    * @return The time to pass between updates on actors
    */
    float GetTimeStep() const;

    /**
    * Resets the scene
    */
    void Reset();

private:

    /**
    * Prevent Copying
    */
    PhysicsScene(const PhysicsScene&) = delete;
    PhysicsScene& operator=(const PhysicsScene&) = delete;

    std::vector<std::unique_ptr<PhysicsObject>> m_actors;   ///< All registered actors in the scene
    float m_timeStep = 0.0f;                                ///< The time to pass between updates on actors
};