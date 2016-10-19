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

void TutorialCreator::CreateTutorial6()
{
    const float middleX = m_data->Size().x / 2.0f;
    const float radius = 40.0f;
    const float offset = 5.0f;
    const float startY = m_data->Size().y - radius - offset;
    const float mass = 2.0f;

    std::unique_ptr<CircleBody> ballDynamic(new CircleBody(
        glm::vec2(middleX - radius - offset, startY),
        glm::vec2(0.0f, 0.0f),
        mass, radius,
        glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)));
    ballDynamic->SetGravity(glm::vec2(0.0f, -9.8f));

    std::unique_ptr<CircleBody> ballStatic(new CircleBody(
        glm::vec2(middleX + radius + offset, startY),
        glm::vec2(0.0f, 0.0f),
        mass, radius,
        glm::vec4(0.0f, 1.0f, 0.5f, 1.0f)));
    ballStatic->SetActive(false);
    ballStatic->SetCollisionResponse(false);

    std::unique_ptr<SpringJoint> joint(new SpringJoint(
        *ballDynamic,
        *ballStatic,
        1.0f, 1.0f));

    m_data->Tweaker().SetGroup("Balls");
    m_data->Tweaker().AddTweakbleObject(ballDynamic.get(), "Dynamic Ball");
    m_data->Tweaker().AddTweakbleObject(ballStatic.get(), "Static Ball");

    m_data->Tweaker().SetGroup("Joint");
    m_data->Tweaker().AddTweakbleJoint(joint.get(), "Joint");

    m_data->Scene().AddActor(std::move(ballDynamic));
    m_data->Scene().AddActor(std::move(ballStatic));
    m_data->Scene().AddActor(std::move(joint));
}