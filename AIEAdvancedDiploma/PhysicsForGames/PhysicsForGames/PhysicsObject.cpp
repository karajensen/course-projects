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

void PhysicsObject::Update(float gravity, float timeStep)
{
    m_gravity = gravity;
    m_timeStep = timeStep;
}

void PhysicsObject::Draw(aie::Renderer2D* renderer)
{
    renderer->setRenderColour(m_colour.r, m_colour.g, m_colour.b, m_colour.a);
}

void PhysicsObject::Debug()
{
}

void PhysicsObject::ResetPosition()
{
}

void PhysicsObject::ApplyForce(const glm::vec2& force)
{
}

void PhysicsObject::ApplyForceToActor(RigidBody* actor, const glm::vec2& force)
{
    throw std::exception("Not Implemented");
}

ShapeType PhysicsObject::GetShapeID() const
{
    return m_shapeID;
}

void PhysicsObject::SetShapeID(ShapeType shapeID)
{
    m_shapeID = shapeID;
}
