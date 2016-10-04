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

/**
* Barrier IDs
*/
namespace Barrier
{
    enum ID
    {
        Left,
        TopLeft,
        TopRight,
        Right,
        BotLeft,
        BotRight,
        Max
    };
}

/**
* Pocket IDs
*/
namespace Pocket
{
    enum ID
    {
        TopLeft,
        TopMid,
        TopRight,
        BotLeft,
        BotMid,
        BotRight,
        Max
    };
}

/**
* Actors for a pool table
*/
struct PoolTable
{
    /**
    * Constructor
    */
    PoolTable()
    {
        barriers.resize(Barrier::Max);
        barriers.assign(barriers.size(), nullptr);
        pockets.resize(Pocket::Max);
        pockets.assign(pockets.size(), nullptr);
    }

    SquareBody* center = nullptr;       ///< Center backdrop of table
    std::vector<SquareBody*> barriers;  ///< Barrier backdrop of table
    std::vector<CircleBody*> pockets;   ///< Pocket bodies of table
    std::vector<CircleBody*> balls;     ///< Balls of the table
};

void TutorialCreator::CreateTutorial4()
{
    PoolTable table;
    m_flts["barrier_size"] = 20.0f;
    m_flts["pocket_size"] = 40.0f;
    m_flts["ball_size"] = 10.0f;
    m_flts["ball_mass"] = 1.0f;

    std::unique_ptr<SquareBody> center(new SquareBody(
        glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f),
        glm::vec2(0, 0), 0.0f,
        glm::vec2(700.0f, 350.0f),
        glm::vec4(0.0f, 0.85f, 0.0, 1.0f)));

    center->SetActive(false);
    center->SetCollidable(false);
    table.center = center.get();
    m_scene.AddActor(std::move(center));

    for (int i = 0; i < int(table.barriers.size()); ++i)
    {
        std::unique_ptr<SquareBody> barrier(new SquareBody(
            glm::vec2(0, 0), glm::vec2(0, 0), 0.0f,
            glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)));

        barrier->SetActive(false);
        barrier->SetCollisionResponse(false);
        table.barriers[i] = barrier.get();
        m_scene.AddActor(std::move(barrier));
    }

    for (int i = 0; i < int(table.pockets.size()); ++i)
    {
        std::unique_ptr<CircleBody> pocket(new CircleBody(
            glm::vec2(0, 0), glm::vec2(0, 0), 
            0.0f, 0.0f, glm::vec4(0, 0, 0, 1)));

        pocket->SetActive(false);
        pocket->SetCollisionResponse(false);
        table.pockets[i] = pocket.get();
        m_scene.AddActor(std::move(pocket));
    }

    for (int i = 0; i < int(table.balls.size()); ++i)
    {
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(0, 0), glm::vec2(0, 0),
            0.0f, 0.0f, glm::vec4(1, 0, 0, 1)));

        for (auto* pocket : table.pockets)
        {
            ball->SetCollisionResponse(
                pocket->GetID(), false, [obj = ball.get()]()
            {
                obj->SetActive(false);
                obj->SetCollidable(false);
                obj->SetVisible(false);
            });
        }

        table.balls[i] = ball.get();
        m_scene.AddActor(std::move(ball));
    }

    auto resetTutorial = [this, table]()
    {
        const glm::vec2& size = table.center->GetSize();
        const glm::vec2& position = table.center->GetPosition();
        const glm::vec2 halfsize(size.x / 2.0f, size.y / 2.0f);
        const glm::vec2 edgeTop(position.x, position.y + halfsize.y);
        const glm::vec2 edgeBot(position.x, position.y - halfsize.y);
        const glm::vec2 edgeLeft(position.x + halfsize.x, position.y);
        const glm::vec2 edgeRight(position.x - halfsize.x, position.y);
        const float barrierSize = m_flts.at("barrier_size");
        const float pocketSize = m_flts.at("pocket_size");
        const float verticalSize = size.y - pocketSize;
        const float horizontalSize = (size.x - (pocketSize * 2.0f)) * 0.5f;
        const float thirdSize = halfsize.x * 0.5f;

        const auto barrierColor = table.center->GetColour() * 0.6f;
        for (auto* barrier : table.barriers)
        {
            barrier->SetColor(barrierColor);
        }

        for (auto* pocket : table.pockets)
        {
            pocket->SetRadius(pocketSize * 0.5f);
        }

        table.pockets[Pocket::TopLeft]->SetPosition(edgeLeft.x, edgeTop.y);
        table.pockets[Pocket::TopMid]->SetPosition(edgeTop.x, edgeTop.y);
        table.pockets[Pocket::TopRight]->SetPosition(edgeRight.x, edgeTop.y);
        table.pockets[Pocket::BotLeft]->SetPosition(edgeLeft.x, edgeBot.y);
        table.pockets[Pocket::BotMid]->SetPosition(edgeTop.x, edgeBot.y);
        table.pockets[Pocket::BotRight]->SetPosition(edgeRight.x, edgeBot.y);

        table.barriers[Barrier::Left]->SetPosition(edgeLeft);
        table.barriers[Barrier::Right]->SetPosition(edgeRight);
        table.barriers[Barrier::TopLeft]->SetPosition(edgeTop.x - thirdSize, edgeTop.y);
        table.barriers[Barrier::TopRight]->SetPosition(edgeTop.x + thirdSize, edgeTop.y);
        table.barriers[Barrier::BotLeft]->SetPosition(edgeBot.x - thirdSize, edgeBot.y);
        table.barriers[Barrier::BotRight]->SetPosition(edgeBot.x + thirdSize, edgeBot.y);

        table.barriers[Barrier::Left]->SetSize(barrierSize, verticalSize);
        table.barriers[Barrier::Right]->SetSize(barrierSize, verticalSize);
        table.barriers[Barrier::TopLeft]->SetSize(horizontalSize, barrierSize);
        table.barriers[Barrier::TopRight]->SetSize(horizontalSize, barrierSize);
        table.barriers[Barrier::BotLeft]->SetSize(horizontalSize, barrierSize);
        table.barriers[Barrier::BotRight]->SetSize(horizontalSize, barrierSize);
    };

    auto resetBallPositions = [this, balls = table.balls]()
    {
        for (auto* ball : balls)
        {
            ball->SetVisible(true);
            ball->SetCollidable(true);
            ball->SetActive(true);
        }
    };

    auto resetBallValues = [this, balls = table.balls]()
    {
        const float ballSize = m_flts.at("ball_size");
        const float ballMass = m_flts.at("ball_size");

        for(auto* ball : balls)
        {
            ball->SetMass(ballMass);
            ball->SetRadius(ballSize);
        }
    };

    m_tweaker->SetGroup("Table");
    m_tweaker->AddTweakbleSquare(table.center, "Board", resetTutorial);
    m_tweaker->AddTweakableFlt("barrier_size", "Barrier Size", 1.0f, 1, resetTutorial);
    m_tweaker->AddTweakableFlt("pocket_size", "Pocket Size", 1.0f, 1, resetTutorial);

    m_tweaker->SetGroup("Balls");
    m_tweaker->AddButton("Reset", resetBallPositions);
    m_tweaker->AddTweakableFlt("ball_mass", "Ball Mass", 0.1f, 3, resetBallValues);
    m_tweaker->AddTweakableFlt("ball_size", "Ball Size", 0.1f, 3, resetBallValues);

    resetTutorial();
    resetBallValues();
    resetBallPositions();
}