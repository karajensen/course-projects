////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <functional>

namespace aie
{
    class Renderer2D;
}

class RigidBody;

class PhysicsObject
{
public:

    /**
    * Constructor
    * @param colour What colour to render the body with
    */
    PhysicsObject(const glm::vec4& colour);

    /**
    * Constructor
    * @param colour What colour to render the body with
    * @param position The position to initialise the body at
    */
    PhysicsObject(const glm::vec4& colour, const glm::vec2& position);

    /**
    * Destructor
    */
    virtual ~PhysicsObject() {}

    /**
    * Updates the body
    * @param timeStep The time between each update call
    */
    virtual void Update(float timeStep);

    /**
    * Renders the body
    * @param renderer The 2D renderer for drawing the body
    */
    virtual void Draw(aie::Renderer2D* renderer);

    /**
    * Outputs debug information about the body
    */
    virtual void Debug();

    /**
    * Resets any current forces on the body
    */
    virtual void ResetForces();

    /**
    * Applies a force to the body
    */
    virtual void ApplyForce(const glm::vec2& force);

    /**
    * Applies a force to the body
    * @param x/y The components of the force to apply
    */
    virtual void ApplyForce(float x, float y);

    /**
    * Applies a force to the body and then an opposite force to the given actor
    * @param actor The body to apply the opposite force to
    * @param force The force to apply to the body
    */
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);

    /**
    * Sets the gravity force
    * @param gravity The gravity to set
    */
    virtual void SetGravity(const glm::vec2& gravity);

    /**
    * Sets the gravity force
    * @param x/y The components of the gravity
    */
    virtual void SetGravity(float x, float y);

    /**
    * Sets the body as active or not
    * @note bodies not active will not render or take part in the physics scene
    */
    void SetActive(bool active);

    /**
    * @return if the body as active or not
    * @note bodies not active will not render or take part in the physics scene
    */
    bool IsActive() const;

    /**
    * Explicitly sets the position of the body
    * @param position The position to set
    */
    void SetPosition(const glm::vec2& position);

    /**
    * Explicitly sets the position of the body
    * @param x/y The components of the position to set
    */
    void SetPosition(float x, float y);

    /**
    * @return the current position of the body
    */
    const glm::vec2& GetPosition() const;

    /**
    * Sets a callback to fire when the body is being updated
    */
    void SetUpdateFn(std::function<void(float)> fn);

protected:
    bool m_isActive = true;                          ///< Whether the body is active or not
    glm::vec4 m_colour;                              ///< Colour to render the body
    glm::vec2 m_position;                            ///< Position of the body
    std::function<void(float)> m_updateFn = nullptr; ///< Callback to fire when the body is being updated

private:

    /**
    * Prevent Copying
    */
    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject& operator=(const PhysicsObject&) = delete;
};