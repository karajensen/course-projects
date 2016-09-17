////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "PhysicsScene.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "Input.h"
#include "Tweaker.h"

TutorialCreator::TutorialCreator(aie::Input& input, PhysicsScene& scene, const glm::ivec2& size)
    : m_scene(scene)
    , m_size(size)
    , m_input(input)
{
}

Tutorial TutorialCreator::Selected() const
{
    return m_currentTutorial;
}

void TutorialCreator::AddToTweaker(Tweaker& tweaker)
{
    tweaker.SetGroup("Tutorials");

    std::vector<const char*> tutorials;
    for (int i = 0; i < Tutorial::TUTORIAL_COUNT; ++i)
    {
        tutorials.push_back(GetName(i));
    }

    tweaker.AddEnumEntry("Selected Tutorial",
        [this]()->int { return m_currentTutorial;  },
        [this, &tweaker](int value) { Create(tweaker, value);  },
        tutorials);

    tweaker.AddButton("Reset Tutorial", [this, &tweaker]()
    {
        Create(tweaker, m_currentTutorial);
    });
}

const char* TutorialCreator::GetName(int tutorial) const
{
    switch (tutorial)
    {
    case TUTORIAL_NONE:
        return "No Tutorial";
    case TUTORIAL_1:
        return "Tutorial 1";
    default:
        return "Unknown";
    }
}

void TutorialCreator::Create(Tweaker& tweaker, int tutorial)
{
    tweaker.Reset();
    m_scene.Reset();
    m_flts.clear();
    m_ints.clear();

    tweaker.SetGroup("Tutorials");

    switch (tutorial)
    {
    case TUTORIAL_NONE:
        break;
    case TUTORIAL_1:
        CreateTutorial1(tweaker);
        break;
    default:
        tutorial = TUTORIAL_NONE;
        LogError("Unsupported tutorial");
    }

    m_currentTutorial = static_cast<Tutorial>(tutorial);
}

void TutorialCreator::CreateTutorial1(Tweaker& tweaker)
{
    const int middleX = m_size.x / 2;
    const int middleY = m_size.y / 2;

    {   // Showing 1st Law
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(middleX, middleY - 80),
            glm::vec2(3.0f, 0),
            3.0f, 40,
            glm::vec4(1, 0, 0, 1)));
        m_scene.AddActor(std::move(ball));
    }

    {   // Showing 2nd Law
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(middleX, middleY + 80),
            glm::vec2(15.0f, 0.0f),
            10.0f, 40,
            glm::vec4(0, 1, 0, 1)));
        ball->SetGravity(0.0f, -9.8f);
        m_scene.AddActor(std::move(ball));
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

        tweaker.AddButton("Demonstrate Third Law", 
            [obj1 = ball1.get(), obj2 = ball2.get()]()
            {
                obj2->ApplyForceToActor(obj1, glm::vec2(1000.0f, 0.0));
            });

        m_scene.AddActor(std::move(ball1));
        m_scene.AddActor(std::move(ball2));
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
            particles.push_back(exhaust.get());
            m_scene.AddActor(std::move(exhaust));
        }

        std::unique_ptr<SquareBody> rocket(new SquareBody(
            glm::vec2(middleX, 100.0f),
            glm::vec2(0.0f, 0.0f),
            3.0f,
            glm::vec2(50.0f, 100.0f),
            glm::vec4(1, 0, 1, 1)));

        rocket->SetGravity(0.0f, -9.8f);
        rocket->SetDamping(0.98f);

        m_flts["time"] = 0.0f;
        m_flts["fuel"] = 10.0f;
        m_flts["forceX"] = 0.0f;
        m_flts["forceY"] = 100.0f;
        m_ints["index"] = 0;

        tweaker.AddFltEntry("Rocket Fuel Level",
            [this]() { return m_flts.at("fuel"); },
            [this](float value) { m_flts.at("fuel") = value; }, 3);

        tweaker.AddFltEntry("Rocket Force X",
            [this]() { return m_flts.at("forceX"); },
            [this](float value) { m_flts.at("forceX") = value; }, 1);

        tweaker.AddFltEntry("Rocket Force Y",
            [this]() { return m_flts.at("forceY"); },
            [this](float value) { m_flts.at("forceY") = value; }, 1);

        rocket->SetUpdateFn([this, particles, obj = rocket.get()](float timestep)
        {
            float& fuel = m_flts.at("fuel");
            if (fuel <= 0.0f)
            {
                return;
            }
            fuel -= 0.01f;

            const auto& forceX = m_flts.at("forceX");
            const auto& forceY = m_flts.at("forceY");

            auto& timepassed = m_flts.at("time");
            timepassed += timestep;
            if(timepassed > 1.0f)
            {
                timepassed = 0.0f;
                auto& index = m_ints.at("index");

                const auto& position = obj->GetPosition();
                particles[index]->SetActive(true);
                particles[index]->SetPosition(position.x, position.y - 50.0f);
                particles[index]->ResetForces();
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
                }
            }

        });

        m_scene.AddActor(std::move(rocket));
    }
}