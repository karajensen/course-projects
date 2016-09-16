////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include <algorithm>
#include <vector>

void PhysicsScene::AddActor(std::unique_ptr<PhysicsObject> actor)
{
    m_actors.push_back(std::move(actor));
}

void PhysicsScene::Update()
{
    for (auto& actor : m_actors)
    {
        actor->Update(m_gravity, m_timeStep);
    }
}

void PhysicsScene::DebugScene()
{
    for (auto& actor : m_actors)
    {
        actor->Debug();
    }
}

void PhysicsScene::Draw(aie::Renderer2D* renderer)
{
    for (auto& actor : m_actors)
    {
        actor->Draw(renderer);
    }
}

void PhysicsScene::SetGravity(float gravity)
{
    m_gravity = gravity;
}

float PhysicsScene::GetGravity() const
{
    return m_gravity;
}

void PhysicsScene::SetTimeStep(float timeStep)
{
    m_timeStep = timeStep;
}

float PhysicsScene::GetTimeStep() const
{
    return m_timeStep;
}
