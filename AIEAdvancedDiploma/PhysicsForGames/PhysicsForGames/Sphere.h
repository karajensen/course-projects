////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sphere.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"
#include "PhysicsObject.h"

class Sphere : public RigidBody
             , public PhysicsObject
{
public:
    virtual void Update(float gravity, float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;
    void SetRadius(float radius);

private:
    float m_radius = 0.0f;  
};