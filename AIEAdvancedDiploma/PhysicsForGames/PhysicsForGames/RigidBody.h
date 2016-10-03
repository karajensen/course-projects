////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:

    /**
    * Constructor
    * @param shape The type of physics body this is
    * @param position The position to initialise the body at
    * @param velocity The initial velocity of the body
    * @param mass The mass of the body
    * @param colour What colour to render the body with
    */
    RigidBody(Shape shape,
              const glm::vec2& position, 
              const glm::vec2& velocity, 
              float mass,
              const glm::vec4& colour);

    /**
    * Updates the body
    * @param timeStep The time between each update call
    */
    virtual void Update(float timeStep) override;

    /**
    * Outputs debug information about the body
    */
    virtual void Debug() override;

    /**
    * Applies a force to the body
    */
    virtual void ApplyForce(const glm::vec2& force) override;

    /**
    * Applies a force to the body
    * @param x/y The components of the force to apply
    */
    virtual void ApplyForce(float x, float y) override;

    /**
    * Applies a force to the body and then an opposite force to the given actor
    * @param actor The body to apply the opposite force to
    * @param force The force to apply to the body
    */
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force) override;

    /**
    * Sets the gravity force
    * @param gravity The gravity to set
    */
    virtual void SetGravity(const glm::vec2& gravity) override;

    /**
    * Sets the gravity force
    * @param x/y The components of the gravity
    */
    virtual void SetGravity(float x, float y) override;

    /**
    * Resets any current forces on the body
    */
    void ResetForces();

    /**
    * Reset the velocity to zero on a body
    */
    void ResetVelocity();

    /**
    * Sets the mass of the body
    */
    void SetMass(float mass);

    /**
    * @return the mass of the body
    */
    float GetMass() const;

    /**
    * Sets how much the velocity of the body will dampen as time passes
    */
    void SetDamping(float damping);

    /**
    * @return how much the velocity of the body will dampen as time passes
    */
    float GetDamping() const;

    /**
    * Sets the velocity of the body
    * @param velocity The velocity to set
    */
    void SetVelocity(const glm::vec2& velocity);

    /**
    * Sets the velocity of the body
    * @param x/y The velocity components to set
    */
    void SetVelocity(float x, float y);

    /**
    * @return the velocity of the body
    */
    const glm::vec2& GetVelocity() const;

    /**
    * @return the position of the body from last tick
    */
    const glm::vec2& GetPreviousPosition() const;

    /**
    * Predicts the position of projectile motion at the given time passed
    * @param gravity The gravity to apply to the body in the y direction
    * @param angle The angle of elevation launched at relative to horizontal in degrees
    * @param speed The speed of the projectile
    * @param timepassed The time to predict at
    */
    static glm::vec2 PredictProjectilePosition(float gravity,
                                               float angle, 
                                               float speed, 
                                               float timepassed);


protected:
    glm::vec2 m_velocity;            ///< Current velocity of the body
    glm::vec2 m_acceleration;        ///< Current accerleration of the body
    glm::vec2 m_gravity;             ///< Gravity force to apply per update
    glm::vec2 m_previousPosition;    ///< Position of the body from last update
    float m_mass = 0.0f;             ///< Mass of the body
    float m_damping = 1.0f;          ///< How much the velocity of the body will dampen as time passes

private:

    /**
    * Prevent Copying
    */
    RigidBody(const RigidBody&) = delete;
    RigidBody& operator=(const RigidBody&) = delete;
};