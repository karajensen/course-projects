////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Plane.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "Renderer2D.h"

Plane::Plane(const glm::vec4& colour) 
    : PhysicsObject(colour)
{
}

void Plane::Update(float timeStep)
{
    PhysicsObject::Update(timeStep);
}

void Plane::Draw(aie::Renderer2D* renderer)
{
    PhysicsObject::Draw(renderer);

    renderer->drawLine(
        m_position.x - m_size / 2.0f,
        m_position.y,
        m_position.x + m_size / 2.0f,
        m_position.y);
}

void Plane::Debug()
{
    PhysicsObject::Debug();
}