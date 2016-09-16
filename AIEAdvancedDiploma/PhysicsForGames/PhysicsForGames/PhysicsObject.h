////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

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
    PhysicsObject(const glm::vec4& colour);
    virtual ~PhysicsObject() {}

    virtual void Update(float gravity, float timeStep);
    virtual void Draw(aie::Renderer2D* renderer);
    virtual void Debug();
    virtual void ApplyForce(const glm::vec2& force);
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);
    ShapeType GetShapeID() const;
    void SetShapeID(ShapeType shapeID);
    void ResetPosition();

protected:
    float m_gravity = 0.0f;
    float m_timeStep = 0.0f;
    ShapeType m_shapeID = NONE;
    glm::vec4 m_colour;
};