////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "RigidBody.h"
#include "Utilities.h"
#include <algorithm>

RigidBody::RigidBody(Shape shape,
                     const glm::vec2& position, 
                     const glm::vec2& velocity, 
                     float mass,
                     const glm::vec4& colour)
    : PhysicsObject(shape, colour)
    , m_velocity(velocity)
    , m_previousPosition(position)
    , m_position(position)
{
    SetMass(mass);
}

void RigidBody::Update(float timeStep)
{
    m_previousPosition = m_position;

    ApplyForce(m_gravity);

    // Euler Integration: X(t + ∆t) = X(t) + X•(t)∆t
    m_velocity = (m_velocity * m_damping) + (m_acceleration * timeStep);
    m_position = m_position + (m_velocity * timeStep);

    Utils::SetZero(m_acceleration);
}

void RigidBody::Debug()
{
    PhysicsObject::Debug();
}

void RigidBody::ApplyForce(float x, float y)
{
    ApplyForce(glm::vec2(x, y));
}

void RigidBody::ApplyForce(const glm::vec2& force)
{
    // F = ma
    m_acceleration += force / m_mass; 
}

void RigidBody::ApplyForceToActor(RigidBody* otherActor, const glm::vec2& force)
{
    ApplyForce(-force);
    otherActor->ApplyForce(force);
}

void RigidBody::SetGravity(const glm::vec2& gravity)
{
    m_gravity = gravity;
}

void RigidBody::SetGravity(float x, float y)
{
    m_gravity.x = x;
    m_gravity.y = y;
}

void RigidBody::ResetForces()
{
    Utils::SetZero(m_acceleration);
}

void RigidBody::ResetVelocity()
{
    Utils::SetZero(m_velocity);
}

void RigidBody::SetMass(float mass)
{
    m_mass = std::max(std::numeric_limits<float>::min(), mass);
}

float RigidBody::GetMass() const
{
    return m_mass;
}

void RigidBody::SetDamping(float damping)
{
    m_damping = damping;
}

float RigidBody::GetDamping() const
{
    return m_damping;
}

void RigidBody::SetVelocity(const glm::vec2& velocity)
{
    m_velocity = velocity;
}

void RigidBody::SetVelocity(float x, float y)
{
    m_velocity.x = x;
    m_velocity.y = y;
}

const glm::vec2& RigidBody::GetVelocity() const
{
    return m_velocity;
}

const glm::vec2& RigidBody::GetPreviousPosition() const
{
    return m_previousPosition;
}

glm::vec2 RigidBody::PredictProjectilePosition(float gravity,
                                               float angle,
                                               float speed, 
                                               float timepassed)
{
    // x = x₀ + stcosӨ
    // y = y₀ + stsinӨ - ¹/₂gt²
    const float rad = DegToRad(angle);
    const float x = speed * timepassed * cos(rad);
    const float y = speed * timepassed * sin(rad) - (0.5f * gravity * (timepassed * timepassed));
    return glm::vec2(x, y);
}

void RigidBody::SetPosition(const glm::vec2& position)
{
    m_position = position;
}

void RigidBody::SetPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

const glm::vec2& RigidBody::GetPosition() const
{
    return m_position;
}
