////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CircleBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CircleBody.h"
#include "Renderer2D.h"
#include "Utilities.h"

CircleBody::CircleBody(const glm::vec2& position,
                       const glm::vec2& velocity, 
                       float mass, 
                       float radius, 
                       const glm::vec4& colour)
    : RigidBody(CIRCLE, position, velocity, mass, colour)
    , m_radius(radius)
{
}

void CircleBody::Update(float timeStep)
{
    RigidBody::Update(timeStep);
}

void CircleBody::Draw(aie::Renderer2D& renderer, bool showDiagnostics)
{
    RigidBody::Draw(renderer, showDiagnostics);
    renderer.drawCircle(m_position.x, m_position.y, m_radius);

    if (showDiagnostics)
    {
        renderer.setRenderColour(1.0f, 1.0f, 1.0f, 1.0f);
        renderer.drawBox(m_position.x, m_position.y, 
            m_radius * 2.0f, m_radius * 2.0f, DegToRad(m_rotation));
    }
}

void CircleBody::SetRadius(float radius)
{
    m_radius = radius;
}

float CircleBody::GetRadius() const
{
    return m_radius;
}