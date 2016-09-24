////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Renderer2D.h"
#include <exception>

PhysicsObject::PhysicsObject(const glm::vec4& colour)
    : m_colour(colour)
{
}

PhysicsObject::PhysicsObject(const glm::vec4& colour, const glm::vec2& position)
    : m_colour(colour)
    , m_position(position)
{
}

void PhysicsObject::Update(float timeStep)
{
    if (m_updateFn != nullptr)
    {
        m_updateFn(timeStep);
    }
}

void PhysicsObject::Draw(aie::Renderer2D* renderer)
{
    renderer->setRenderColour(m_colour.r, m_colour.g, m_colour.b, m_colour.a);
}

void PhysicsObject::Debug()
{
}

void PhysicsObject::ApplyForce(const glm::vec2& force)
{
}

void PhysicsObject::ApplyForce(float x, float y)
{
}

void PhysicsObject::ApplyForceToActor(RigidBody* actor, const glm::vec2& force)
{
}

void PhysicsObject::SetGravity(const glm::vec2& gravity)
{
}

void PhysicsObject::SetGravity(float x, float y)
{
}

void PhysicsObject::ResetForces()
{
}

void PhysicsObject::SetPosition(const glm::vec2& position)
{
    m_position = position;
}

void PhysicsObject::SetPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

const glm::vec2& PhysicsObject::GetPosition() const
{
    return m_position;
}

void PhysicsObject::SetUpdateFn(std::function<void(float)> fn)
{
    m_updateFn = fn;
}

void PhysicsObject::SetActive(bool active)
{
    m_isActive = active;
}

bool PhysicsObject::IsActive() const
{
    return m_isActive;
}

void PhysicsObject::SetVisible(bool visible)
{
    m_isVisible = visible;
}

bool PhysicsObject::IsVisible() const
{
    return m_isVisible;
}