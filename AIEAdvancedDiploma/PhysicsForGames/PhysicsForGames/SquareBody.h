////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SquareBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"

class SquareBody : public RigidBody
{
public:
    SquareBody(const glm::vec2& position, 
               const glm::vec2& velocity, 
               float mass, 
               const glm::vec4& colour);

    virtual void Update(float gravity, float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;

private:
    glm::vec2 m_size;
};