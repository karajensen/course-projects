////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Plane.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include "Renderer2D.h"
#include "glm/glm.hpp"

Plane::Plane(const glm::vec2& normal,
             float distance, 
             float size,
             const glm::vec4& colour)
    : PhysicsObject(PLANE, colour)
    , m_distance(distance)
    , m_size(size)
{
    SetNormal(normal);
}

void Plane::Update(float timeStep)
{
}

void Plane::Draw(aie::Renderer2D& renderer, bool showDiagnostics)
{
    PhysicsObject::Draw(renderer, showDiagnostics);

    const glm::vec2 centrePoint = m_normal * m_distance;

    // Need to rotate normal 90 degrees around z
    const glm::vec2 parallel(m_normal.y, -m_normal.x);
    const glm::vec2 start = centrePoint + (parallel * m_size);
    const glm::vec2 end = centrePoint - (parallel * m_size);

    renderer.drawLine(start.x, start.y, end.x, end.y, 1.0f, 0.0f);
}

float Plane::GetDistance() const
{
    return m_distance;
}

void Plane::SetDistance(float distance)
{
    m_distance = distance;
}

const glm::vec2& Plane::GetNormal() const
{
    return m_normal;
}

void Plane::SetNormal(const glm::vec2& normal)
{
    m_normal = glm::normalize(normal);
}

void Plane::SetNormal(float x, float y)
{
    m_normal.x = x;
    m_normal.y = y;
    m_normal = glm::normalize(m_normal);
}

void Plane::SetSize(float size)
{
    m_size = size;
}

void Plane::SetNormalX(float x)
{
    m_normal.x = x;
    m_normal = glm::normalize(m_normal);
}

void Plane::SetNormalY(float y)
{
    m_normal.y = y;
    m_normal = glm::normalize(m_normal);
}

float Plane::GetSize() const
{
    return m_size;
}