////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsObject.h"
#include "RigidBody.h"

void PhysicsObject::Update(float gravity, float timeStep)
{
    m_gravity = gravity;
    m_timeStep = timeStep;
}

void PhysicsObject::ResetPosition()
{
}

void PhysicsObject::ApplyForce(const glm::vec2& force)
{
}

void PhysicsObject::ApplyForceToActor(RigidBody* actor, const glm::vec2& force)
{
}

ShapeType PhysicsObject::GetShapeID() const
{
    return m_shapeID;
}

void PhysicsObject::SetShapeID(ShapeType shapeID)
{
    m_shapeID = shapeID;
}
