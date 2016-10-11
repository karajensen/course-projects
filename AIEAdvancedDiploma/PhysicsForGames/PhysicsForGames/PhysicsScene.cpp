////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsScene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Utilities.h"
#include "Tweaker.h"
#include "CollisionSolver.h"

#include <algorithm>
#include <vector>

PhysicsScene::PhysicsScene(aie::Renderer2D& renderer)
    : m_timeStep(1.0f / 60.0f)
    , m_solver(new CollisionSolver())
    , m_renderer(renderer)
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

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
    auto fn = [actor](const std::unique_ptr<PhysicsObject>& a) { return a.get() == actor; };
    m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), fn), m_actors.end());
}

void PhysicsScene::Update()
{
    for (auto& actor : m_actors)
    {
        if (actor->IsActive())
        {
            actor->PreUpdate(m_timeStep);
            actor->Update(m_timeStep);
        }
        else
        {
            actor->InactiveUpdate();
        }
    }

    CheckForCollision();

    for (auto& actor : m_actors)
    {
        if (actor->IsActive())
        {
            actor->PostUpdate(m_timeStep);
        }
    }
}

void PhysicsScene::Draw()
{
    m_solver->Draw(m_renderer);

    // Draw in reverse order to preserve depth
    for (auto itr = m_actors.rbegin(); itr != m_actors.rend(); ++itr)
    {
        if ((*itr)->IsVisible())
        {
            (*itr)->Draw(m_renderer, m_actorDiagnostics);
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

void PhysicsScene::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddFltEntry("Time Step", 
        [this]() { return m_timeStep; },
        [this](float value) { m_timeStep = value; },
        0.01f, 3);

    tweaker.AddBoolEntry("Actor Diagnosics",
        [this]() { return m_actorDiagnostics; },
        [this](bool value) { m_actorDiagnostics = value;  });

    m_solver->AddToTweaker(tweaker);
}

void PhysicsScene::CheckForCollision()
{
    const int actorCount = static_cast<int>(m_actors.size());

    // need to check for collisions against all objects except this one
    for (int outer = 0; outer < actorCount - 1; outer++)
    {
        for (int inner = outer + 1; inner < actorCount; inner++)
        {
            PhysicsObject& object1 = *m_actors[outer];
            PhysicsObject& object2 = *m_actors[inner];
            m_solver->SolveCollision(object1, object2);
        }
    }
}