////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Renderer2D.h"
#include <exception>

unsigned int PhysicsObject::sm_idCounter = 0;

PhysicsObject::PhysicsObject(Shape shape, const glm::vec4& colour)
    : m_colour(colour)
    , m_shape(shape)
    , m_id(sm_idCounter++)
    , m_defaultResponse(std::make_pair(true, nullptr))
{
}

void PhysicsObject::PreUpdate(float timeStep)
{
    if (m_isActive)
    {
        m_inCollision = false;
        if (m_preUpdate)
        {
            m_preUpdate(timeStep);
        }
    }
    else
    {
        if (m_inactiveFn)
        {
            m_inactiveFn();
        }
    }
}

void PhysicsObject::PostUpdate(float timeStep)
{
    if (m_isActive && m_postUpdate)
    {
        m_postUpdate(timeStep);
    }
}

void PhysicsObject::Draw(aie::Renderer2D& renderer)
{
    renderer.setRenderColour(m_colour.r, m_colour.g, m_colour.b, m_colour.a);
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

void PhysicsObject::SetPreUpdateFn(std::function<void(float)> fn)
{
    m_preUpdate = fn;
}

void PhysicsObject::SetPostUpdateFn(std::function<void(float)> fn)
{
    m_postUpdate = fn;
}

void PhysicsObject::SetInactiveFn(std::function<void(void)> fn)
{
    m_inactiveFn = fn;
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

PhysicsObject::Shape PhysicsObject::GetShape() const
{
    return m_shape;
}

void PhysicsObject::SetColliding()
{
    m_inCollision = true;
}

bool PhysicsObject::IsColliding() const
{
    return m_inCollision;
}

void PhysicsObject::SetColor(const glm::vec4& colour)
{
    m_colour = colour;
}

void PhysicsObject::SetColor(float r, float g, float b, float a)
{
    m_colour.r = r;
    m_colour.g = g;
    m_colour.b = b;
    m_colour.a = a;
}

const glm::vec4& PhysicsObject::GetColour() const
{
    return m_colour;
}

void PhysicsObject::SetCollisionResponse(bool shouldCollide,
                                         std::function<void(void)> fn)
{
    m_defaultResponse = std::make_pair(shouldCollide, fn);
}

void PhysicsObject::SetCollisionResponse(unsigned int id, 
                                         bool shouldCollide,
                                         std::function<void(void)> fn)
{
    m_response[id] = std::make_pair(shouldCollide, fn);
}

const PhysicsObject::CollisionResponse& PhysicsObject::GetCollisionResponse(unsigned int id) const
{
    return m_response.find(id) != m_response.end() 
        ? m_response.at(id) : m_defaultResponse;
}

unsigned int PhysicsObject::GetID() const
{
    return m_id;
}

void PhysicsObject::SetCollidable(bool isCollidable)
{
    m_isCollidable = isCollidable;
}

bool PhysicsObject::IsCollidable() const
{
    return m_isCollidable;
}