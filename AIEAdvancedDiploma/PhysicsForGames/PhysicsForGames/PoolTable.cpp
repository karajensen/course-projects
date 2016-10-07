////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PoolTable.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PoolTable.h"
#include "TutorialData.h"
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

void PoolTable::CreateBoard(TutorialData& data, Actors& actors)
{
    // Pool table center
    std::unique_ptr<SquareBody> centerBody(new SquareBody(
        glm::vec2(data.size.x / 2.0f, data.size.y / 2.0f),
        glm::vec2(0, 0), 0.0f,
        glm::vec2(700.0f, 350.0f),
        glm::vec4(0.0f, 0.85f, 0.0, 1.0f)));

    centerBody->SetActive(false);
    centerBody->SetCollidable(false);
    actors.center = centerBody.get();
    data.scene->AddActor(std::move(centerBody));

    // Pool table barriers
    for (int i = 0; i < int(actors.barriers.size()); ++i)
    {
        std::unique_ptr<SquareBody> barrier(new SquareBody(
            glm::vec2(0, 0), glm::vec2(0, 0), 0.0f,
            glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)));

        barrier->SetActive(false);
        barrier->SetCollisionResponse(false);
        actors.barriers[i] = barrier.get();
        data.scene->AddActor(std::move(barrier));
    }

    // Pool table pockets
    for (int i = 0; i < int(actors.pockets.size()); ++i)
    {
        std::unique_ptr<CircleBody> pocket(new CircleBody(
            glm::vec2(0, 0), glm::vec2(0, 0),
            0.0f, 0.0f, glm::vec4(0, 0, 0, 1)));

        pocket->SetActive(false);
        pocket->SetCollisionResponse(false);
        actors.pockets[i] = pocket.get();
        data.scene->AddActor(std::move(pocket));
    }
}

void PoolTable::CreateBalls(TutorialData& data, Actors& actors)
{
    bool isRed = true;
    const glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);

    for (int i = 0; i < int(actors.balls.size()); ++i)
    {
        std::unique_ptr<CircleBody> ball(new CircleBody(
            glm::vec2(0, 0), glm::vec2(0, 0),
            0.0f, 0.0f, glm::vec4(0, 0, 0, 1)));

        for (auto* pocket : actors.pockets)
        {
            ball->SetCollisionResponse(
                pocket->GetID(), false, [obj = ball.get()]()
            {
                obj->SetActive(false);
                obj->SetCollidable(false);
                obj->SetVisible(false);
            });
        }

        if (i > 0 && i != 5)
        {
            ball->SetColor(isRed ? red : blue);
            isRed = !isRed;
        }

        actors.balls[i] = ball.get();
        data.scene->AddActor(std::move(ball));
    }

    actors.playerBall = actors.balls[0];
    actors.playerBall->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void PoolTable::CreateCue(TutorialData& data, Actors& actors)
{
    std::unique_ptr<SquareBody> cue(new SquareBody(
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0, 0), 0.0f,
        glm::vec2(0.0f, 0.0f),
        glm::vec4(1.0f, 1.0f, 0.5f, 1.0f)));

    std::unique_ptr<SquareBody> cuetip(new SquareBody(
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0, 0), 0.0f,
        glm::vec2(0.0f, 0.0f),
        glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)));

    cuetip->SetActive(false);
    cuetip->SetCollidable(false);
    cue->SetActive(false);
    cue->SetCollidable(false);

    cue->SetInactiveFn([&data, ball = actors.playerBall, obj = cue.get(), tip = cuetip.get()]()
    {
        obj->SetVisible(false);
        tip->SetVisible(false);

        if (data.input->IsMouseDown())
        {
            const bool inverted = data.ints.at("cue_inverted") != 0;
            const float size = data.flts.at("cue_size");
            const auto end = data.input->Convert(data.input->MouseDownPosition());
            const auto start = data.input->Convert(data.input->MousePosition());
            obj->MakeFromLine(inverted ? start : end, inverted ? end : start, size);
            tip->SetPosition(inverted ? start : end);
            tip->SetSize(size, size);
            tip->SetRotation(obj->GetRotation());
            obj->SetVisible(true);
            tip->SetVisible(true);
        }

        if (data.input->WasMouseReleased())
        {
            const bool inverted = data.ints.at("cue_inverted") != 0;
            const auto hitBuffer = 30.0f;
            const auto start = data.input->Convert(data.input->MousePosition());
            const auto end = data.input->Convert(data.input->MouseDownPosition());

            if (glm::length((inverted ? start : end) - ball->GetPosition()) < ball->GetRadius() + hitBuffer)
            {
                const float force = data.flts.at("cue_force");
                ball->ApplyForce((inverted ? start - end : end - start) * force);
            }
        }
    });

    data.scene->AddActor(std::move(cue));
    data.scene->AddActor(std::move(cuetip));
}

void PoolTable::Create(TutorialData& data)
{
    Actors actors;
    actors.barriers.resize(Barrier::Max);
    actors.barriers.assign(actors.barriers.size(), nullptr);
    actors.pockets.resize(Pocket::Max);
    actors.pockets.assign(actors.pockets.size(), nullptr);
    actors.balls.resize(16);
    actors.balls.assign(actors.balls.size(), nullptr);

    data.flts["barrier_size"] = 20.0f;
    data.flts["pocket_size"] = 40.0f;
    data.flts["ball_size"] = 11.0f;
    data.flts["ball_mass"] = 0.5f;
    data.flts["player_start"] = 120.0f;
    data.flts["balls_start"] = -120.0f;
    data.flts["cue_size"] = 10.0f;
    data.flts["cue_force"] = 10.0f;
    data.ints["cue_inverted"] = 0;

    CreateBoard(data, actors);
    CreateBalls(data, actors);
    CreateCue(data, actors);

    auto resetTable = [actors, &data]()
    {
        const glm::vec2& size = actors.center->GetSize();
        const glm::vec2& position = actors.center->GetPosition();
        const glm::vec2 halfsize(size.x / 2.0f, size.y / 2.0f);
        const glm::vec2 edgeTop(position.x, position.y + halfsize.y);
        const glm::vec2 edgeBot(position.x, position.y - halfsize.y);
        const glm::vec2 edgeLeft(position.x - halfsize.x, position.y);
        const glm::vec2 edgeRight(position.x + halfsize.x, position.y);
        const float barrierSize = data.flts.at("barrier_size");
        const float pocketSize = data.flts.at("pocket_size");
        const float verticalSize = size.y - pocketSize;
        const float horizontalSize = (size.x - (pocketSize * 2.0f)) * 0.5f;
        const float thirdSize = halfsize.x * 0.5f;

        const auto barrierColor = actors.center->GetColour() * 0.6f;
        for (auto* barrier : actors.barriers)
        {
            barrier->SetColor(barrierColor);
        }

        for (auto* pocket : actors.pockets)
        {
            pocket->SetRadius(pocketSize * 0.5f);
        }

        actors.pockets[Pocket::TopLeft]->SetPosition(edgeLeft.x, edgeTop.y);
        actors.pockets[Pocket::TopMid]->SetPosition(edgeTop.x, edgeTop.y);
        actors.pockets[Pocket::TopRight]->SetPosition(edgeRight.x, edgeTop.y);
        actors.pockets[Pocket::BotLeft]->SetPosition(edgeLeft.x, edgeBot.y);
        actors.pockets[Pocket::BotMid]->SetPosition(edgeTop.x, edgeBot.y);
        actors.pockets[Pocket::BotRight]->SetPosition(edgeRight.x, edgeBot.y);

        actors.barriers[Barrier::Left]->SetPosition(edgeLeft);
        actors.barriers[Barrier::Right]->SetPosition(edgeRight);
        actors.barriers[Barrier::TopLeft]->SetPosition(edgeTop.x - thirdSize, edgeTop.y);
        actors.barriers[Barrier::TopRight]->SetPosition(edgeTop.x + thirdSize, edgeTop.y);
        actors.barriers[Barrier::BotLeft]->SetPosition(edgeBot.x - thirdSize, edgeBot.y);
        actors.barriers[Barrier::BotRight]->SetPosition(edgeBot.x + thirdSize, edgeBot.y);

        actors.barriers[Barrier::Left]->SetSize(barrierSize, verticalSize);
        actors.barriers[Barrier::Right]->SetSize(barrierSize, verticalSize);
        actors.barriers[Barrier::TopLeft]->SetSize(horizontalSize, barrierSize);
        actors.barriers[Barrier::TopRight]->SetSize(horizontalSize, barrierSize);
        actors.barriers[Barrier::BotLeft]->SetSize(horizontalSize, barrierSize);
        actors.barriers[Barrier::BotRight]->SetSize(horizontalSize, barrierSize);
    };

    auto resetBallPositions = [actors, &data]()
    {
        const glm::vec2& size = actors.center->GetSize();
        const glm::vec2& position = actors.center->GetPosition();
        const float ballStart = data.flts.at("balls_start");
        const float playerStart = data.flts.at("player_start");
        const float ballSize = data.flts.at("ball_size") * 2.0f;
        const float halfSize = size.x * 0.5f;
        const float xOffset = -3.0f;

        for (auto* ball : actors.balls)
        {
            ball->ResetForces();
            ball->ResetVelocity();
            ball->SetVisible(true);
            ball->SetCollidable(true);
            ball->SetActive(true);
        }

        int counter = 0;
        float xPosition = position.x + halfSize + ballStart;
        for (int i = 1; i < (int)actors.balls.size(); i += counter)
        {
            counter++;
            const float height = counter * ballSize;
            float yPosition = position.y + (height * 0.5f) - (ballSize * 0.5f);
            for (int j = 0; j < counter; ++j)
            {
                const int index = i + j;
                actors.balls[index]->SetPosition(xPosition, yPosition);
                yPosition -= ballSize;
            }
            xPosition += ballSize + xOffset;
        }

        actors.playerBall->SetPosition(position.x - halfSize + playerStart, position.y);
    };

    auto resetBallValues = [actors, &data]()
    {
        const float ballSize = data.flts.at("ball_size");
        const float ballMass = data.flts.at("ball_mass");

        for(auto* ball : actors.balls)
        {
            ball->SetMass(ballMass);
            ball->SetRadius(ballSize);
        }
    };

    data.tweaker->SetGroup("Table");
    data.tweaker->AddTweakbleSquare(actors.center, "Board", resetTable);
    data.tweaker->AddTweakableFlt("barrier_size", "Barrier Size", 1.0f, 1, resetTable);
    data.tweaker->AddTweakableFlt("pocket_size", "Pocket Size", 1.0f, 1, resetTable);

    data.tweaker->SetGroup("Player");
    data.tweaker->AddTweakableFlt("cue_size", "Cue Size", 1.0f, 1);
    data.tweaker->AddTweakableFlt("cue_force", "Cue Force", 1.0f, 1);
    data.tweaker->AddTweakableBool("cue_inverted", "Cue Inverted");

    data.tweaker->SetGroup("Balls");
    data.tweaker->AddButton("Reset", resetBallPositions);
    data.tweaker->AddTweakableFlt("ball_mass", "Ball Mass", 0.1f, 3, resetBallValues);
    data.tweaker->AddTweakableFlt("ball_size", "Ball Size", 0.1f, 3, resetBallValues);
    data.tweaker->AddTweakableFlt("player_start", "Player Start", 0.1f, 3, resetBallPositions);
    data.tweaker->AddTweakableFlt("balls_start", "Balls Start", 0.1f, 3, resetBallPositions);

    resetTable();
    resetBallValues();
    resetBallPositions();
}