////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial2.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "TutorialTweaker.h"
#include "PhysicsScene.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "Input.h"
#include "glm/glm.hpp"

void TutorialCreator::CreateTutorial2()
{
    m_data->CreateInt("step_count", 20);
    m_data->CreateFlt("step_size", 0.1f);
    m_data->CreateFlt("step_angle", 60.0f);
    m_data->CreateFlt("step_speed", 115.0f);

    auto resetTutorial = [this]()
    {
        m_data->Scene().Reset();

        const auto steps = m_data->GetInt("step_count");
        const auto stepSize = m_data->GetFlt("step_size");
        const auto angle = m_data->GetFlt("step_angle");
        const auto speed = m_data->GetFlt("step_speed");

        const auto rad = DegToRad(angle);
        const glm::vec2 velocity(speed * cos(rad), speed * sin(rad));

        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(0.0f, 0.0f),
            velocity,
            1.0f, 20,
            glm::vec4(0, 1, 0, 1)));

        ball->SetCollidable(false);
        ball->SetGravity(0.0f, -9.8f);
        ball->SetPreUpdateFn([size = m_data->Size(), obj = ball.get(), velocity](float timestep)
        {
            const auto buffer = 10;
            const auto& position = obj->GetPosition();
            if (position.x > size.x + buffer || 
                position.y > size.y + buffer ||
                position.x < -buffer ||
                position.y < -buffer)
            {
                obj->SetPosition(0, 0);
                obj->SetVelocity(velocity);
            }
        });

        m_data->Scene().AddActor(std::move(ball));

        for (float i = 0; i < steps; i += stepSize)
        {
            std::unique_ptr<SquareBody> step(new SquareBody(
                RigidBody::PredictProjectilePosition(9.8f, angle, speed, i),
                glm::vec2(0, 0),
                0.0f, glm::vec2(4.0f, 4.0f),
                glm::vec4(1, 0, 0, 1)));
        
            step->SetActive(false);
            step->SetCollidable(false);
            m_data->Scene().AddActor(std::move(step));
        }
    };

    m_tweaker->SetGroup("Prediction Curve");
    m_tweaker->AddTweakableFlt("step_angle", "Step Angle", 0.1f, 3, resetTutorial);
    m_tweaker->AddTweakableFlt("step_speed", "Step Speed", 0.1f, 3, resetTutorial);
    m_tweaker->AddTweakableFlt("step_size", "Step Size", 0.1f, 0.1f, 1.0f, 3, resetTutorial);
    m_tweaker->AddTweakableInt("step_count", "Step Count", 0, 100, resetTutorial);

    resetTutorial();
}