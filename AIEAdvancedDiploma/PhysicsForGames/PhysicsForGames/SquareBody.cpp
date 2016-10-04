////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SquareBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SquareBody.h"
#include "Renderer2D.h"

SquareBody::SquareBody(const glm::vec2& position,
                       const glm::vec2& velocity, 
                       float mass,
                       const glm::vec2& size,
                       const glm::vec4& colour)
    : RigidBody(SQUARE, position, velocity, mass, colour)
    , m_size(size)
{
}

void SquareBody::Update(float timeStep)
{
    RigidBody::Update(timeStep);
}

void SquareBody::Draw(aie::Renderer2D* renderer)
{
    RigidBody::Draw(renderer);

    renderer->drawBox(std::round(m_position.x), 
        std::round(m_position.y), std::round(m_size.x), std::round(m_size.y));
}

void SquareBody::Debug()
{
    RigidBody::Debug();
}

void SquareBody::SetSize(const glm::vec2& size)
{
    m_size = size;
}

void SquareBody::SetSize(float x, float y)
{
    m_size.x = x;
    m_size.y = y;
}

const glm::vec2& SquareBody::GetSize() const
{
    return m_size;
}