////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial2.cpp
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

void TutorialCreator::CreateTutorial2(Tweaker& tweaker)
{
    m_ints["step_count"] = 20;
    m_flts["step_size"] = 0.1f;
    m_flts["step_angle"] = 60.0f;
    m_flts["step_speed"] = 115.0f;

    auto resetTutorial = [this]()
    {
        m_scene.Reset();

        const auto steps = m_ints.at("step_count");
        const auto stepSize = m_flts.at("step_size");
        const auto angle = m_flts.at("step_angle");
        const auto speed = m_flts.at("step_speed");

        const auto rad = DegToRad(angle);
        const glm::vec2 velocity(speed * cos(rad), speed * sin(rad));

        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(0.0f, 0.0f),
            velocity,
            1.0f, 20,
            glm::vec4(0, 1, 0, 1)));

        ball->SetGravity(0.0f, -9.8f);
        ball->SetPreUpdateFn([size = m_size, obj = ball.get(), velocity](float timestep)
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

        m_scene.AddActor(std::move(ball));

        for (float i = 0; i < steps; i += stepSize)
        {
            std::unique_ptr<SquareBody> step(new SquareBody(
                RigidBody::PredictProjectilePosition(9.8f, angle, speed, i),
                glm::vec2(0, 0),
                0.0f, glm::vec2(4.0f, 4.0f),
                glm::vec4(1, 0, 0, 1)));
        
            step->SetActive(false);
            m_scene.AddActor(std::move(step));
        }
    };

    tweaker.SetGroup("Prediction Curve");
    AddTweakableFlt(tweaker, "step_angle", "Step Angle", 0.1f, 3, resetTutorial);
    AddTweakableFlt(tweaker, "step_speed", "Step Speed", 0.1f, 3, resetTutorial);
    AddTweakableFlt(tweaker, "step_size", "Step Size", 0.1f, 0.1f, 1.0f, 3, resetTutorial);
    AddTweakableInt(tweaker, "step_count", "Step Count", 0, 100, resetTutorial);

    resetTutorial();
}