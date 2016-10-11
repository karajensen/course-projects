////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SquareBody.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SquareBody.h"
#include "Renderer2D.h"
#include "glm/glm.hpp"
#include "Utilities.h"
#include <algorithm>

SquareBody::SquareBody(const glm::vec2& position,
                       const glm::vec2& velocity, 
                       float mass,
                       const glm::vec2& size,
                       const glm::vec4& colour)
    : RigidBody(SQUARE, position, velocity, mass, colour)
    , m_size(size)
{
}

void SquareBody::MakeFromLine(const glm::vec2& start, const glm::vec2& end, float size)
{
    const glm::vec2 direction(start - end);
    m_position = end + (direction * 0.5f);
    m_size.y = size;
    m_size.x = glm::length(direction);

    const float dot = glm::dot(direction, glm::vec2(direction.y < 0.0f ? -1.0f : 1.0f, 0.0f));
    m_rotation = RadToDeg(m_size.x == 0.0f ? 0.0f : acos(dot / m_size.x));
}

void SquareBody::Update(float timeStep)
{
    RigidBody::Update(timeStep);
}

void SquareBody::Draw(aie::Renderer2D& renderer, bool showDiagnostics)
{
    RigidBody::Draw(renderer, showDiagnostics);

    renderer.drawBox(std::round(m_position.x), std::round(m_position.y), 
        std::round(m_size.x), std::round(m_size.y), DegToRad(m_rotation));
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

void SquareBody::SetRadius(float radius)
{
    SetSize(radius * 2.0f, radius * 2.0f);
}

float SquareBody::GetRadius() const
{
    return std::max(m_size.x, m_size.y) * 0.5f;
}