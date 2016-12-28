////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial6.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "TutorialTweaker.h"
#include "PhysicsScene.h"
#include "CircleBody.h"
#include "SpringJoint.h"
#include "Utilities.h"
#include "glm/glm.hpp"
#include <algorithm>
#include <string>

void TutorialCreator::CreateTutorial6()
{
    auto& tweaker = m_data->Tweaker();
    auto& scene = m_data->Scene();

    // Create the interactible rope
    {
        std::vector<CircleBody*> balls;
        const int maxBalls = 20;

        m_data->CreateFlt("rope_radius", 10.0f);
        m_data->CreateFlt("rope_mass", 2.0f);
        m_data->CreateFlt("rope_offset", 2.25f);
        m_data->CreateFlt("rope_drag", 0.0f);
        m_data->CreateFlt("rope_elasticity", 1.0f);
        m_data->CreateFlt("rope_k", 40.0f);
        m_data->CreateFlt("rope_damping", 0.01f);
        m_data->CreateInt("rope_count", maxBalls);
        
        std::vector<SpringJoint*> joints;
        
        std::unique_ptr<CircleBody> ballStatic(new CircleBody(
            glm::vec2(0, 0), glm::vec2(0, 0),
            0, 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
        
        ballStatic->SetActive(false);
        ballStatic->MakeStatic();
        auto* ball1 = ballStatic.get();
        balls.push_back(ball1);
        scene.AddActor(std::move(ballStatic));
        
        for (int i = 1; i < maxBalls; i++)
        {
            std::unique_ptr<CircleBody> ballDynamic(new CircleBody(
                glm::vec2(0, 0), glm::vec2(0, 0),
                0, 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
            
            ballDynamic->CanRotateDynamically(false);
            ballDynamic->SetGravity(0.0f, -9.8f);
            auto* ball2 = ballDynamic.get();
            balls.push_back(ball2);
            scene.AddActor(std::move(ballDynamic));
            
            std::unique_ptr<SpringJoint> joint(new SpringJoint(
                ball1, ball2, 0.0f, 0.0f,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
            
            joints.push_back(joint.get());
            scene.AddActor(std::move(joint));
            
            ball1 = ball2;
        }
        
        auto updateJoints = [joints, this]()
        {
            const auto k = m_data->GetFlt("rope_k");
            const auto damping = std::min(1.0f, std::max(m_data->GetFlt("rope_damping"), 0.0f));
            m_data->SetFlt("rope_damping", damping);

            for (auto joint : joints)
            {
                joint->SetDamping(damping);
                joint->SetSpringCoefficient(k);
            }
        };

        auto createRope = [balls, joints, this]()
        {
            const auto radius = m_data->GetFlt("rope_radius");
            const auto offset = radius * m_data->GetFlt("rope_offset");
            const float startX = m_data->Size().x * 0.5f + 50.0f;
            const float startY = m_data->Size().y - 100.0f;
        
            for(int i = 0; i < (int)balls.size(); ++i)
            {
                balls[i]->ResetForces();
                balls[i]->ResetVelocity();
                balls[i]->SetPosition(startX - i * offset, startY);
                balls[i]->SetRadius(radius);
            }

            for (auto joint : joints)
            {
                joint->Reset();
            }
        };

        auto updateRope = [balls, maxBalls, this]()
        {
            const auto mass = m_data->GetFlt("rope_mass");
            const auto drag = m_data->GetFlt("rope_drag");
            const auto elasticity = m_data->GetFlt("rope_elasticity");

            const auto count = std::max(1, std::min(m_data->GetInt("rope_count"), maxBalls));
            m_data->SetInt("rope_count", count);

            for (int i = 0; i < (int)balls.size(); ++i)
            {
                balls[i]->SetActive(i < count);
                balls[i]->SetVisible(i < count);
                balls[i]->SetCollidable(i < count);

                balls[i]->SetMass(mass);
                balls[i]->SetLinearDrag(drag);
                balls[i]->SetElasticity(elasticity);
            }
        };

        auto resetRope = [updateRope, createRope, updateJoints]()
        {
            createRope();
            updateRope();
            updateJoints();
        };
        
        tweaker.SetGroup("Rope");
        tweaker.AddButton("Reset Rope", resetRope);
        tweaker.AddTweakableInt("rope_count", "Count", updateRope);
        tweaker.AddTweakableFlt("rope_radius", "Radius", 0.01f, 3, createRope);
        tweaker.AddTweakableFlt("rope_mass", "Mass", 0.01f, 3, updateRope);
        tweaker.AddTweakableFlt("rope_offset", "Offset", 0.01f, 3, createRope);
        tweaker.AddTweakableFlt("rope_drag", "Drag", 0.01f, 3, updateRope);
        tweaker.AddTweakableFlt("rope_elasticity", "Elasticity", 0.01f, 3, updateRope);
        tweaker.AddTweakableFlt("rope_k", "K", 0.01f, 3, updateJoints);
        tweaker.AddTweakableFlt("rope_damping", "Damping", 0.01f, 3, updateJoints);

        resetRope();
    }

    // Create the rope chain
    {
        const int maxBalls = 15;
        const float radius = 10.0f;
        const float mass = 2.0f;
        const float offset = 2.1f;
        const float k = 40.0;
        const float damping = 0.01f;
        const float startX = 350.0f;
        const float startY = 200.0f;

        std::unique_ptr<CircleBody> ballStatic(new CircleBody(
            glm::vec2(startX, startY), glm::vec2(0, 0),
            mass, radius, glm::vec4(1.0f, 0.0f, 0.5f, 1.0f)));

        ballStatic->SetActive(false);
        ballStatic->MakeStatic();
        auto* ball1 = ballStatic.get();
        scene.AddActor(std::move(ballStatic));

        for (int i = 1; i < maxBalls; i++)
        {
            std::unique_ptr<CircleBody> ballDynamic(new CircleBody(
                glm::vec2(startX - i * radius * offset, startY), glm::vec2(0, 0),
                mass, radius, glm::vec4(1.0f, 0.0f, 0.5f, 1.0f)));

            ballDynamic->CanRotateDynamically(false);
            ballDynamic->SetGravity(0.0f, -9.8f);
            auto* ball2 = ballDynamic.get();
            scene.AddActor(std::move(ballDynamic));

            std::unique_ptr<SpringJoint> joint(new SpringJoint(
                ball1, ball2, 0.0f, 0.0f,
                glm::vec4(1.0f, 0.0f, 0.5f, 1.0f)));
            joint->SetDamping(damping);
            joint->SetSpringCoefficient(k);

            scene.AddActor(std::move(joint));
            ball1 = ball2;
        }

        ball1->SetActive(false);
        ball1->MakeStatic();

        std::unique_ptr<CircleBody> ballDynamic(new CircleBody(
            glm::vec2(startX - 50.0f, startY + 100.0f), glm::vec2(0, 0),
            mass, 30.0f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));

        ballDynamic->SetGravity(0.0f, -9.8f);
        scene.AddActor(std::move(ballDynamic));
    }

    // Create the single springs to the right
    {   
        const float mass = 2.0f;
        const float radius = 30.0f;
        const float startX = m_data->Size().x - 200.0f;
        float startY = m_data->Size().y - 100.0f;
        tweaker.SetGroup("Joints");

        // Green ball example
        auto greenDynamic = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(startX - radius - 5.0f, startY),
            glm::vec2(0.0f, 0.0f),
            mass, radius,
            glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)));
        greenDynamic->SetGravity(glm::vec2(0.0f, -9.8f));
        greenDynamic->CanRotateDynamically(false);

        auto greenStatic = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(startX + radius + 5.0f, startY),
            glm::vec2(0.0f, 0.0f),
            mass, radius,
            glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)));
        greenStatic->SetActive(false);
        greenStatic->MakeStatic();

        auto greenJoint = std::unique_ptr<SpringJoint>(new SpringJoint(
            greenDynamic.get(),
            greenStatic.get(),
            1.5f, 1.0f,
            glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)));

        tweaker.AddTweakbleJoint(greenJoint.get(), "Green");
        scene.AddActor(std::move(greenStatic));
        scene.AddActor(std::move(greenDynamic));
        scene.AddActor(std::move(greenJoint));

        startY -= 200.0f;

        // Red ball example
        const float redStartX = startX + radius + 5.0f;
        auto redStatic = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(redStartX, startY),
            glm::vec2(0.0f, 0.0f),
            mass, radius,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));
        redStatic->SetActive(false);
        redStatic->MakeStatic();

        auto redDynamic1 = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(redStartX, startY - radius * 2.0f),
            glm::vec2(0.0f, 0.0f),
            mass * 0.5f, radius * 0.5f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));
        redDynamic1->SetGravity(glm::vec2(0.0f, -9.8f));
        redDynamic1->CanRotateDynamically(false);

        auto redDynamic2 = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(redStartX - radius * 2.0f, startY + radius * 2.0f),
            glm::vec2(0.0f, 0.0f),
            mass * 0.5f, radius * 0.5f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));
        redDynamic2->SetGravity(glm::vec2(0.0f, -9.8f));
        redDynamic2->CanRotateDynamically(false);

        auto redDynamic3 = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(redStartX + radius * 2.0f, startY + radius * 2.0f),
            glm::vec2(0.0f, 0.0f),
            mass * 0.5f, radius * 0.5f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));
        redDynamic3->SetGravity(glm::vec2(0.0f, -9.8f));
        redDynamic3->CanRotateDynamically(false);

        auto redJoint1 = std::unique_ptr<SpringJoint>(new SpringJoint(
            redStatic.get(),
            redDynamic1.get(),
            1.0f, 0.8f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));

        auto redJoint2 = std::unique_ptr<SpringJoint>(new SpringJoint(
            redDynamic1.get(),
            redDynamic2.get(),
            1.0f, 1.0f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));

        auto redJoint3 = std::unique_ptr<SpringJoint>(new SpringJoint(
            redDynamic1.get(),
            redDynamic3.get(),
            1.0f, 1.0f,
            glm::vec4(1.0f, 0.2f, 0.0f, 1.0f)));

        tweaker.AddTweakbleJoint(redJoint1.get(), "Red 1");
        tweaker.AddTweakbleJoint(redJoint2.get(), "Red 2");
        tweaker.AddTweakbleJoint(redJoint3.get(), "Red 3");
        scene.AddActor(std::move(redStatic));
        scene.AddActor(std::move(redDynamic1));
        scene.AddActor(std::move(redDynamic2));
        scene.AddActor(std::move(redDynamic3));
        scene.AddActor(std::move(redJoint1));
        scene.AddActor(std::move(redJoint2));
        scene.AddActor(std::move(redJoint3));

        startY -= 200.0f;

        // Blue ball example
        auto blueDynamic = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(startX - radius - 5.0f, startY),
            glm::vec2(0.0f, 0.0f),
            mass, radius,
            glm::vec4(0.0f, 0.2f, 1.0f, 1.0f)));
        blueDynamic->SetGravity(glm::vec2(0.0f, -9.8f));
        blueDynamic->CanRotateDynamically(false);

        auto blueStatic = std::unique_ptr<CircleBody>(new CircleBody(
            glm::vec2(startX + radius + 5.0f, startY),
            glm::vec2(0.0f, 0.0f),
            mass, radius,
            glm::vec4(0.0f, 0.2f, 1.0f, 1.0f)));
        blueStatic->SetActive(false);
        blueStatic->MakeStatic();

        auto blueJoint = std::unique_ptr<SpringJoint>(new SpringJoint(
            blueDynamic.get(),
            blueStatic.get(),
            0.5f, 0.5f,
            glm::vec4(0.0f, 0.2f, 1.0f, 1.0f)));

        tweaker.AddTweakbleJoint(blueJoint.get(), "Blue");
        scene.AddActor(std::move(blueDynamic));
        scene.AddActor(std::move(blueStatic));
        scene.AddActor(std::move(blueJoint));
    }
}