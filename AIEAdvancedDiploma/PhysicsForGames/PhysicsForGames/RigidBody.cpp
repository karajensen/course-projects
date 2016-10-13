////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - RigidBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "RigidBody.h"
#include "Utilities.h"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <algorithm>
#include <numeric>

const float MIN_LINEAR_THRESHOLD = std::numeric_limits<float>::epsilon();
const float MIN_ROTATION_THRESHOLD = std::numeric_limits<float>::epsilon();

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
    if (m_canMove)
    {
        m_previousPosition = m_position;

        ApplyForce(m_gravity);

        m_position += m_velocity * timeStep;
        m_velocity += m_acceleration * timeStep;
        m_velocity *= 1.0f - m_linearDrag;

        if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
        {
            Utils::SetZero(m_velocity);
        }

        Utils::SetZero(m_acceleration);
    }

    if (m_canRotate)
    {
        m_rotation += m_angularVelocity * timeStep;
        m_angularVelocity += m_angularAcceleration * timeStep;
        m_angularVelocity *= 1.0f - m_angularDrag;

        if (m_rotation > 360.0f)
        {
            m_rotation = 0.0f;
        }

        if (m_angularVelocity < MIN_ROTATION_THRESHOLD)
        {
            m_angularVelocity = 0.0f;
        }

        m_angularAcceleration = 0.0f;
    }
}

float RigidBody::GetRotation() const
{
    return m_rotation;
}

void RigidBody::SetRotation(float rotation)
{
    m_rotation = rotation;
}

void RigidBody::ApplyForce(float x, float y)
{
    ApplyForce(glm::vec2(x, y));
}

void RigidBody::ApplyRotationalForce(float force)
{
    // τ = iα
    m_angularAcceleration += force / m_moi;
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
    m_angularAcceleration = 0.0f;
}

void RigidBody::ResetVelocity()
{
    Utils::SetZero(m_velocity);
    m_angularVelocity = 0.0f;
}

void RigidBody::SetMass(float mass)
{
    m_mass = std::max(std::numeric_limits<float>::min(), mass);
}

float RigidBody::GetMass() const
{
    return m_mass;
}

void RigidBody::SetLinearDrag(float drag)
{
    m_linearDrag = std::min(1.0f, std::max(0.0f, drag));
}

float RigidBody::GetLinearDrag() const 
{
    return m_linearDrag;
}

void RigidBody::SetAngularDrag(float drag)
{
    m_angularDrag = std::min(1.0f, std::max(0.0f, drag));
}

float RigidBody::GetAngularDrag() const
{
    return m_angularDrag;
}

void RigidBody::SetAngularVelocity(float velocity)
{
    m_angularVelocity = velocity;
}

float RigidBody::GetAngularVelocity() const
{
    return m_angularVelocity;
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

float RigidBody::GetElasticity() const
{
    return m_elasticity;
}

void RigidBody::SetElasticity(float elasticity)
{
    m_elasticity = std::min(1.0f, std::max(0.0f, elasticity));
}

void RigidBody::SetMomentOfInertia(float moi)
{
    m_moi = std::max(std::numeric_limits<float>::min(), moi);
}

float RigidBody::GetMomentOfInertia() const
{
    return m_moi;
}

bool RigidBody::CanRotate() const
{
    return m_canRotate;
}

void RigidBody::CanRotate(bool rotate)
{
    m_canRotate = rotate;
}

bool RigidBody::CanMove() const
{
    return m_canMove;
}

void RigidBody::CanMove(bool move)
{
    m_canMove = move;
}