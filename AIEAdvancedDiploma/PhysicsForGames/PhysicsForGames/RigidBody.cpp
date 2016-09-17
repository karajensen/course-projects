////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "RigidBody.h"
#include "Utilities.h"
#include <algorithm>

RigidBody::RigidBody(const glm::vec2& position, 
                     const glm::vec2& velocity, 
                     float mass,
                     const glm::vec4& colour)
    : PhysicsObject(colour, position)
    , m_velocity(velocity)
{
    SetMass(mass);
}

void RigidBody::Update(float timeStep)
{
    PhysicsObject::Update(timeStep);

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
