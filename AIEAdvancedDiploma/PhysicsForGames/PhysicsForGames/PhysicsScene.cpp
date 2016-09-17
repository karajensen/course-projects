////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Utilities.h"

#include <algorithm>
#include <vector>

PhysicsScene::PhysicsScene()
    : m_timeStep(1.0f / 60.0f)
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::Reset()
{
    m_actors.clear();
}

void PhysicsScene::AddActor(std::unique_ptr<PhysicsObject> actor)
{
    m_actors.push_back(std::move(actor));
}

void PhysicsScene::Update()
{
    for (auto& actor : m_actors)
    {
        if (actor->IsActive())
        {
            actor->Update(m_timeStep);
        }
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
        if (actor->IsActive())
        {
            actor->Draw(renderer);
        }
    }
}

void PhysicsScene::SetGravity(float x, float y)
{
    for (auto& actor : m_actors)
    {
        actor->SetGravity(x, y);
    }
}

void PhysicsScene::SetGravity(const glm::vec2& gravity)
{
    for (auto& actor : m_actors)
    {
        actor->SetGravity(gravity);
    }
}

void PhysicsScene::SetTimeStep(float timeStep)
{
    m_timeStep = timeStep;
}

float PhysicsScene::GetTimeStep() const
{
    return m_timeStep;
}