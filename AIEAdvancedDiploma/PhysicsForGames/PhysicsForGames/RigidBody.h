////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
    RigidBody(const glm::vec2& position, 
              const glm::vec2& velocity, 
              float mass,
              const glm::vec4& colour);

    virtual void Update(float timeStep) override;
    virtual void Debug() override;
    virtual void ApplyForce(const glm::vec2& force) override;
    virtual void ApplyForce(float x, float y) override;
    virtual void ApplyForceToActor(RigidBody* otherActor, const glm::vec2& force) override;
    virtual void SetGravity(const glm::vec2& gravity) override;
    virtual void SetGravity(float x, float y) override;
    virtual void ResetForces() override;

    void SetMass(float mass);
    float GetMass() const;
    void SetDamping(float damping);
    float GetDamping() const;

protected:
    glm::vec2 m_velocity;
    glm::vec2 m_acceleration;
    glm::vec2 m_gravity;
    float m_mass = 0.0f;
    float m_damping = 1.0f;

private:
    RigidBody(const RigidBody&) = delete;
    RigidBody& operator=(const RigidBody&) = delete;
};