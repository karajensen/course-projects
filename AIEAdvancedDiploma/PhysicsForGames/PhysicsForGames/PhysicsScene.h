////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "glm/vec2.hpp"

namespace aie
{
    class Renderer2D;
}

class PhysicsObject;

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    void AddActor(std::unique_ptr<PhysicsObject> actor);
    void Update();
    void DebugScene();
    void Draw(aie::Renderer2D* renderer);
    void SetGravity(float x, float y);
    void SetGravity(const glm::vec2& gravity);
    void SetTimeStep(float timeStep);
    float GetTimeStep() const;
    void Reset();

private:
    PhysicsScene(const PhysicsScene&) = delete;
    PhysicsScene& operator=(const PhysicsScene&) = delete;

    std::vector<std::unique_ptr<PhysicsObject>> m_actors;
    float m_timeStep = 0.0f;
};