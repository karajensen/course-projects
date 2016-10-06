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

class Tweaker;
class PhysicsObject;
class CollisionSolver;

class PhysicsScene
{
public:

    /**
    * Constructor
    * @param renderer The 2D renderer for drawing the scene
    */
    PhysicsScene(aie::Renderer2D& renderer);

    /**
    * Destructor
    */
    ~PhysicsScene();

    /**
    * Adds a new actor to the scene
    */
    void AddActor(std::unique_ptr<PhysicsObject> actor);

    /**
    * Removes an actor to the scene
    * @note do not call this during an update or render call
    */
    void RemoveActor(PhysicsObject* actor);

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
    */
    void Draw();

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

    /**
    * Adds the tutorial creator to the tweakbar
    */
    void AddToTweaker(Tweaker& tweaker);

private:

    /**
    * Prevent Copying
    */
    PhysicsScene(const PhysicsScene&) = delete;
    PhysicsScene& operator=(const PhysicsScene&) = delete;

    /**
    * Checks for collisions between actors
    */
    void CheckForCollision();

    std::unique_ptr<CollisionSolver> m_solver;              ///< Actor collision solver
    std::vector<std::unique_ptr<PhysicsObject>> m_actors;   ///< All registered actors in the scene
    float m_timeStep = 0.0f;                                ///< The time to pass between updates on actors
    aie::Renderer2D& m_renderer;                            ///< The 2D renderer for drawing the scene
};