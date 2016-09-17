////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Plane.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
    Plane(const glm::vec4& colour);

    virtual void Update(float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;

private:
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;

    float m_size = 0.0f;
};