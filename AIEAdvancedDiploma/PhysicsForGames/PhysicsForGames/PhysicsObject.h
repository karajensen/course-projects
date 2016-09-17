////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <functional>

namespace aie
{
    class Renderer2D;
}

class RigidBody;

class PhysicsObject
{
public:
    PhysicsObject(const glm::vec4& colour);
    PhysicsObject(const glm::vec4& colour, const glm::vec2& position);
    virtual ~PhysicsObject() {}

    virtual void Update(float timeStep);
    virtual void Draw(aie::Renderer2D* renderer);
    virtual void Debug();
    virtual void ResetForces();
    virtual void ApplyForce(const glm::vec2& force);
    virtual void ApplyForce(float x, float y);
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);
    virtual void SetGravity(const glm::vec2& gravity);
    virtual void SetGravity(float x, float y);

    void SetActive(bool active);
    bool IsActive() const;
    void SetPosition(const glm::vec2& position);
    void SetPosition(float x, float y);
    const glm::vec2& GetPosition() const;
    void SetUpdateFn(std::function<void(float)> fn);

protected:
    bool m_isActive = true;
    glm::vec4 m_colour;
    glm::vec2 m_position;
    std::function<void(float)> m_updateFn = nullptr;

private:
    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject& operator=(const PhysicsObject&) = delete;
};