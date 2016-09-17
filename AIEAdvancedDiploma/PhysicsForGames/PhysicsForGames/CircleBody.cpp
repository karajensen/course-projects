////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CircleBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CircleBody.h"
#include "Renderer2D.h"

CircleBody::CircleBody(const glm::vec2& position,
                       const glm::vec2& velocity, 
                       float mass, 
                       float radius, 
                       const glm::vec4& colour)
    : RigidBody(position, velocity, mass, colour)
    , m_radius(radius)
{
}

void CircleBody::Update(float timeStep)
{
    RigidBody::Update(timeStep);
}

void CircleBody::Draw(aie::Renderer2D* renderer)
{
    RigidBody::Draw(renderer);

    renderer->drawCircle(m_position.x, m_position.y, m_radius);
}

void CircleBody::SetRadius(float radius)
{
    m_radius = radius;
}

void CircleBody::Debug()
{
    RigidBody::Debug();
}