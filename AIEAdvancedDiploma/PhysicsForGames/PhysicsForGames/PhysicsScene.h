////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

namespace aie
{
    class Renderer2D;
}

class PhysicsObject;

class PhysicsScene
{
public:
    void AddActor(std::unique_ptr<PhysicsObject> actor);
    void Update();
    void DebugScene();
    void Draw(aie::Renderer2D* renderer);
    void SetGravity(float gravity);
    float GetGravity() const;
    void SetTimeStep(float timeStep);
    float GetTimeStep() const;

private:
    std::vector<std::unique_ptr<PhysicsObject>> m_actors;
    float m_gravity = 0.0f;
    float m_timeStep = 0.0f;
};