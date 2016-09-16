////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sphere.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Sphere.h"

void Sphere::Update(float gravity, float timeStep)
{
    PhysicsObject::Update(gravity, timeStep);
}

void Sphere::Draw(aie::Renderer2D* renderer)
{

}

void Sphere::SetRadius(float radius)
{
    m_radius = radius;
}

void Sphere::Debug()
{

}