////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sphere.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"

class CircleBody : public RigidBody
{
public:
    CircleBody(const glm::vec2& position, 
               const glm::vec2& velocity, 
               float mass, 
               float radius, 
               const glm::vec4& colour);

    virtual void Update(float timeStep) override;
    virtual void Draw(aie::Renderer2D* renderer) override;
    virtual void Debug() override;
    void SetRadius(float radius);

private:
    CircleBody(const CircleBody&) = delete;
    CircleBody& operator=(const CircleBody&) = delete;

    float m_radius = 0.0f;  
};