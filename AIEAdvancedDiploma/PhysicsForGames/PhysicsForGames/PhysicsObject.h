////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"

namespace aie
{
    class Renderer2D;
}

enum ShapeType
{
    PLANE = 0,
    SPHERE = 1,
    BOX = 2,
    NONE = 3
};

class RigidBody;

class PhysicsObject
{
public:
    virtual void Update(float gravity, float timeStep);
    virtual void Draw(aie::Renderer2D* renderer) = 0;
    virtual void Debug() = 0;
    virtual void ResetPosition();
    virtual void ApplyForce(const glm::vec2& force);
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);
    ShapeType GetShapeID() const;
    void SetShapeID(ShapeType shapeID);

protected:
    float m_gravity = 0.0f;
    float m_timeStep = 0.0f;
    ShapeType m_shapeID = NONE;
};