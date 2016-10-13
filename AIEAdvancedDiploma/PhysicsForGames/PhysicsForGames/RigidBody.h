////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

/**
* Physics body that can be manipulated by forces
*/
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
    * Applies a rotational force to the body
    */
    void ApplyRotationalForce(float force);

    /**
    * Applies a force to the body
    */
    void ApplyForce(const glm::vec2& force);

    /**
    * Applies a force to the body
    * @param x/y The components of the force to apply
    */
    void ApplyForce(float x, float y);

    /**
    * Applies a force to the body and then an opposite force to the given actor
    * @param actor The body to apply the opposite force to
    * @param force The force to apply to the body
    */
    void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);

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
    * Sets how much the linear velocity of the body will dampen as time passes
    * @param drag how much the linear velocity of the body will dampen as time passes
    */
    void SetLinearDrag(float drag);

    /**
    * @return how much the linear velocity of the body will dampen as time passes
    */
    float GetLinearDrag() const;

    /**
    * Sets how much the linear velocity of the body will dampen as time passes
    * @param drag how much the linear velocity of the body will dampen as time passes
    */
    void SetAngularDrag(float drag);

    /**
    * @return how much the linear velocity of the body will dampen as time passes
    */
    float GetAngularDrag() const;

    /**
    * Sets the angular velocity of the body
    * @param velocity The angular velocity to set
    */
    void SetAngularVelocity(float velocity);

    /**
    * @return the angular velocity of the body
    */
    float GetAngularVelocity() const;

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
    * @return the rotation of the body in degrees
    */
    float GetRotation() const;

    /**
    * Sets the rotation of the body in radians
    * @param rotation The rotation of the body in degrees
    */
    void SetRotation(float rotation);

    /**
    * @return how much energy is lost during collision
    */
    float GetElasticity() const;

    /**
    * Sets how much energy is lost during collision
    * @param elasticity how much energy is lost during collision
    */
    void SetElasticity(float elasticity);

    /**
    * Sets the radius of the body
    */
    virtual void SetRadius(float radius) = 0;

    /**
    * @return the radius of the body
    */
    virtual float GetRadius() const = 0;

    /**
    * Sets the moment of inertia
    * @param moi The moment of inertia
    */
    void SetMomentOfInertia(float moi);

    /**
    * Gets the moment of inertia
    */
    float GetMomentOfInertia() const;

    /**
    * @return whether this body is allowed to rotate
    */
    bool CanRotate() const;

    /**
    * Sets whether this body is allowed to rotate
    */
    void CanRotate(bool rotate);

    /**
    * @return whether this body is allowed to translate
    */
    bool CanMove() const;

    /**
    * Sets whether this body is allowed to translate
    */
    void CanMove(bool move);

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

    glm::vec2 m_position;                ///< Position of the body
    glm::vec2 m_velocity;                ///< Current velocity of the body
    glm::vec2 m_acceleration;            ///< Current acceleration of the body
    glm::vec2 m_gravity;                 ///< Gravity force to apply per update
    glm::vec2 m_previousPosition;        ///< Position of the body from last update
    float m_angularAcceleration = 0.0f;  ///< Current angular acceleration of the body
    float m_angularVelocity = 0.0f;      ///< Angular velocity of the body
    float m_rotation = 0.0f;             ///< Rotation of the body in degrees
    float m_elasticity = 1.0f;           ///< How much energy is lost during collision
    float m_mass = 0.0f;                 ///< Mass of the body
    float m_linearDrag = 0.0f;           ///< How much the linear velocity of the body will dampen as time passes
    float m_angularDrag = 0.0f;          ///< How much the angular velocity of the body will dampen as time passes
    float m_moi = 1.0f;                  ///< Moment of inertia, how resistant to changes in rotation
    bool m_canRotate = true;             ///< Whether the body can rotate
    bool m_canMove = true;               ///< Whether the body can translate

private:

    /**
    * Prevent Copying
    */
    RigidBody(const RigidBody&) = delete;
    RigidBody& operator=(const RigidBody&) = delete;
};