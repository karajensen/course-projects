////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Box.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"
#include "PhysicsObject.h"

class Box : public RigidBody
          , public PhysicsObject
{
public:
    virtual void Update(float gravity, float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;

private:
    glm::vec2 m_size;
};