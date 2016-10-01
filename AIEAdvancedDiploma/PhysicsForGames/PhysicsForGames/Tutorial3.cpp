////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial3.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "PhysicsScene.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "Input.h"
#include "Tweaker.h"
#include "glm/glm.hpp"

void TutorialCreator::CreateTutorial3(Tweaker& tweaker)
{
    tweaker.SetGroup("Planes");

    auto addPlaneToTweaker = [&tweaker](Plane* p, const std::string& name)
    {
        tweaker.AddFltEntry((name + " Normal X").c_str(),
            [p](){ return p->GetNormal().x; },
            [p](float value){ p->SetNormalX(value); },
            0.1f, 3);

        tweaker.AddFltEntry((name + " Normal Y").c_str(),
            [p](){ return p->GetNormal().y; },
            [p](float value){ p->SetNormalY(value); },
            0.1f, 3);

        tweaker.AddFltEntry((name + " Distance").c_str(),
            [p]() { return p->GetDistance(); },
            [p](float value) { p->SetDistance(value); },
            0.1f, 3);

        tweaker.AddFltEntry((name + " Size").c_str(),
            [p]() { return p->GetSize(); },
            [p](float value) { p->SetSize(value); },
            0.1f, 3);
    };

    std::unique_ptr<Plane> plane1(new Plane(
        glm::vec2(0.565f, 0.825f), 300.0f, 500.0f,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0)));
    plane1->SetActive(false);
    addPlaneToTweaker(plane1.get(), "P1");
    m_scene.AddActor(std::move(plane1));

    std::unique_ptr<Plane> plane2(new Plane(
        glm::vec2(-0.290f, 0.957f), -100.0f, 1500.0f,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0)));
    plane2->SetActive(false);
    addPlaneToTweaker(plane2.get(), "P2");
    m_scene.AddActor(std::move(plane2));

    auto createBall = [this](float radius, float pX, float pY, float vX, float vY, bool gravity)
    {
        const glm::vec4 normalColor(1.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec4 collisionColor(0.0f, 1.0f, 0.0f, 1.0f);

        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(pX, pY),
            glm::vec2(vX, vY),
            1.0f, radius, normalColor));

        if (gravity)
        {
            ball->SetGravity(glm::vec2(0.0f, -9.8f));
        }
        
        ball->SetPostUpdateFn([normalColor, collisionColor, obj = ball.get()](float timestep)
        {
            obj->SetColor(obj->IsColliding() ? collisionColor : normalColor);
        });

        m_scene.AddActor(std::move(ball));
    };

    createBall(20.0f, 0.0f, m_size.y - 40.0f, 30.0f, 0.0f, false);
    createBall(30.0f, m_size.x + 30.0f, m_size.y - 60.0f, -30.0f, 0.0f, false);

    auto spawnBalls = [this, createBall]()
    {
        const int maxBalls = 5;
        const float startY = m_size.y * 0.75f;
        const float startX = m_size.x / static_cast<float>(maxBalls + 1);
        const int minRadius = 30;
        const int maxRadius = 60;

        for (int i = 0; i < maxBalls; ++i)
        {
            const float radius = static_cast<float>(
                (rand() % (maxRadius - minRadius + 1)) + minRadius);

            float xOffset = static_cast<float>(
                (rand() % (maxRadius - minRadius + 1)) + minRadius);
            xOffset *= (rand() % 2) == 0 ? -1.0f : 1.0f;

            createBall(radius, startX * (i + 1) + xOffset, startY, 0.0f, 0.0, true);
        }
    };

    tweaker.AddButton("Spawn Balls", spawnBalls);

    spawnBalls();
}