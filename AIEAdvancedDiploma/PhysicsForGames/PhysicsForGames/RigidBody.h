////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
    RigidBody(const glm::vec2& position, 
              const glm::vec2& velocity, 
              float rotation, 
              float mass,
              const glm::vec4& colour);

    virtual void Update(float gravity, float timeStep) override;
    virtual void Debug() override;
    virtual void ApplyForce(const glm::vec2& force) override;
    virtual void ApplyForceToActor(RigidBody* otherActor, const glm::vec2& force) override;
    void SetPosition(const glm::vec2& position);
    const glm::vec2& GetPosition() const;
    void SetVelocity(const glm::vec2& velocity);
    const glm::vec2& GetVelocity() const;
    void SetMass(float mass);
    float GetMass() const;
    void SetRotation2D(float rotation);
    float GetRotation2D() const;

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass = 0.0f;
    float m_rotation2D = 0.0f;
};