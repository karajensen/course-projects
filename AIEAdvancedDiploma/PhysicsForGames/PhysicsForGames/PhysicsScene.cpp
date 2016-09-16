////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include <algorithm>
#include <vector>

void PhysicsScene::AddActor(PhysicsObject* actor)
{
    m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
    m_actors.erase(std::remove(m_actors.begin(), m_actors.end(), actor), m_actors.end());
}

void PhysicsScene::GetActors() const
{

}

void PhysicsScene::Update()
{
    for (PhysicsObject* actor : m_actors)
    {
        actor->Update(m_gravity, m_timeStep);
    }
}

void PhysicsScene::DebugScene()
{
    for (PhysicsObject* actor : m_actors)
    {
        actor->Debug();
    }
}

void PhysicsScene::Draw(aie::Renderer2D* renderer)
{
    for (PhysicsObject* actor : m_actors)
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
