////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "RigidBody.h"

RigidBody::RigidBody(const glm::vec2& position, 
                     const glm::vec2& velocity, 
                     float rotation,
                     float mass,
                     const glm::vec4& colour)
    : PhysicsObject(colour)
    , m_position(position)
    , m_velocity(velocity)
    , m_rotation2D(rotation)
    , m_mass(mass)
{
}

void RigidBody::Update(float gravity, float timeStep)
{
    PhysicsObject::Update(gravity, timeStep);
}

void RigidBody::Debug()
{
    PhysicsObject::Debug();
}

void RigidBody::ApplyForce(const glm::vec2& force)
{
    PhysicsObject::ApplyForce(force);
}

void RigidBody::ApplyForceToActor(RigidBody* otherActor, const glm::vec2& force)
{

}

void RigidBody::SetPosition(const glm::vec2& position)
{
    m_position = position;
}

void RigidBody::SetVelocity(const glm::vec2& velocity)
{
    m_velocity = velocity;
}

void RigidBody::SetMass(float mass)
{
    m_mass = mass;
}

void RigidBody::SetRotation2D(float rotation)
{
    m_rotation2D = rotation;
}

float RigidBody::GetRotation2D() const
{
    return m_rotation2D;
}

float RigidBody::GetMass() const
{
    return m_mass;
}

const glm::vec2& RigidBody::GetPosition() const
{
    return m_position;
}

const glm::vec2& RigidBody::GetVelocity() const
{
    return m_velocity;
}