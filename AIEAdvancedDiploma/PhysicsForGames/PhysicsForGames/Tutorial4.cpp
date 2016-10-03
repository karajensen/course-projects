////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial4.cpp
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

void TutorialCreator::CreateTutorial4()
{
    m_tweaker->SetGroup("Table");

    std::unique_ptr<SquareBody> boardBody(new SquareBody(
        glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f),
        glm::vec2(0, 0), 0.0f,
        glm::vec2(700.0f, 350.0f),
        glm::vec4(0.0f, 0.85f, 0.0, 1.0f)));
    auto* board = boardBody.get();

    auto resetTutorial = [this, board]()
    {
        const glm::vec2& size = board->GetSize();
        const glm::vec2& position = board->GetPosition();
        const glm::vec2 halfsize(size.x / 2.0f, size.y / 2.0f);
        const glm::vec2 edgeTop(position.x, position.y + halfsize.y);
        const glm::vec2 edgeBot(position.x, position.y - halfsize.y);
        const glm::vec2 edgeLeft(position.x + halfsize.x, position.y);
        const glm::vec2 edgeRight(position.x - halfsize.x, position.y);
    };

    m_tweaker->AddTweakbleSquare(board, "Board");
    m_scene.AddActor(std::move(boardBody));
}