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
               const glm::vec2& size,
               const glm::vec4& colour);

    virtual void Update(float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;

private:
    SquareBody(const SquareBody&) = delete;
    SquareBody& operator=(const SquareBody&) = delete;

    glm::vec2 m_size;
};