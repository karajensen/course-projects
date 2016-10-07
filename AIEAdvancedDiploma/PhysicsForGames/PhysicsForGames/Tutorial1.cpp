////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial1.cpp
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

void TutorialCreator::CreateTutorial1()
{
    const int middleX = m_data.size.x / 2;
    const int middleY = m_data.size.y / 2;

    {   // Showing 1st Law
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(middleX, middleY - 80),
            glm::vec2(3.0f, 0),
            3.0f, 40,
            glm::vec4(1, 0, 0, 1)));
        ball->SetCollidable(false);
        m_data.scene->AddActor(std::move(ball));
    }

    {   // Showing 2nd Law
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(middleX, middleY + 80),
            glm::vec2(15.0f, 0.0f),
            10.0f, 40,
            glm::vec4(0, 1, 0, 1)));
        ball->SetGravity(0.0f, -9.8f);
        ball->SetCollidable(false);
        m_data.scene->AddActor(std::move(ball));
    }

    {   // Showing 3rd Law
        std::unique_ptr<CircleBody> ball1(new CircleBody(
            glm::vec2(middleX + 40, middleY),
            glm::vec2(0.0f, 0.0f),
            3.0f, 40,
            glm::vec4(0, 0, 1, 1)));

        std::unique_ptr<CircleBody> ball2(new CircleBody(
            glm::vec2(middleX - 40, middleY),
            glm::vec2(0.0f, 0.0f),
            6.0f, 40,
            glm::vec4(0, 1, 1, 1)));

        m_tweaker->AddButton("Demonstrate Third Law", 
            [obj1 = ball1.get(), obj2 = ball2.get()]()
            {
                obj2->ApplyForceToActor(obj1, glm::vec2(1000.0f, 0.0));
            });

        ball1->SetCollidable(false);
        ball2->SetCollidable(false);
        m_data.scene->AddActor(std::move(ball1));
        m_data.scene->AddActor(std::move(ball2));
    }

    { // Simulating a rocket motor
        std::vector<CircleBody*> particles;
        for (int i = 0; i < 6; ++i)
        {
            std::unique_ptr<CircleBody> exhaust(new CircleBody(
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),
                1.0f, 10,
                glm::vec4(0.8, 0.8, 0.8, 1)));
            exhaust->SetActive(false);
            exhaust->SetVisible(false);
            exhaust->SetCollidable(false);
            particles.push_back(exhaust.get());
            m_data.scene->AddActor(std::move(exhaust));
        }

        std::unique_ptr<CircleBody> rocket(new CircleBody(
            glm::vec2(middleX, 100.0f),
            glm::vec2(0.0f, 0.0f),
            3.0f, 40.0f,
            glm::vec4(1, 0, 1, 1)));

        rocket->SetCollidable(false);
        rocket->SetGravity(0.0f, -9.8f);
        rocket->SetDamping(0.98f);

        m_data.flts["time"] = 0.0f;
        m_data.flts["fuel"] = 10.0f;
        m_data.flts["forceX"] = 25.0f;
        m_data.flts["forceY"] = 100.0f;
        m_data.ints["index"] = 0;

        m_tweaker->AddTweakableFlt("fuel", "Rocket Fuel Level", 0.1f, 3);
        m_tweaker->AddTweakableFlt("forceX", "Rocket Force X", 0.1f, 1);
        m_tweaker->AddTweakableFlt("forceY", "Rocket Force Y", 0.1f, 1);

        rocket->SetPreUpdateFn([this, particles, obj = rocket.get()](float timestep)
        {
            float& fuel = m_data.flts.at("fuel");
            if (fuel <= 0.0f)
            {
                return;
            }

            fuel -= 0.01f;

            const auto& forceX = m_data.flts.at("forceX");
            const auto& forceY = m_data.flts.at("forceY");

            auto& timepassed = m_data.flts.at("time");
            timepassed += timestep;
            if(timepassed > 1.0f)
            {
                timepassed = 0.0f;
                auto& index = m_data.ints.at("index");

                glm::vec2 position = obj->GetPosition();
                glm::vec2 direction(forceX, forceY);
                if (!Utils::IsZero(direction))
                {
                    position -= glm::normalize(direction) * 40.0f;
                }

                particles[index]->SetActive(true);
                particles[index]->SetVisible(true);
                particles[index]->SetPosition(position.x, position.y);
                particles[index]->ResetForces();
                particles[index]->ResetVelocity();
                obj->ApplyForceToActor(particles[index], glm::vec2(forceX, forceY));
                
                index = index + 1 >= static_cast<int>(particles.size()) ? 0 : index + 1;
            }
            else
            {
                obj->ApplyForce(forceX, forceY);
            }

            if (fuel <= 0.0f)
            {
                for (auto* particle : particles)
                {
                    particle->SetActive(false);
                    particle->SetVisible(false);
                }
            }
        });

        m_data.scene->AddActor(std::move(rocket));
    }
}