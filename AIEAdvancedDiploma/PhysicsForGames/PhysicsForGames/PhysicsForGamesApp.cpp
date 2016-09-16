////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsForGamesApp.h"
#include "Font.h"
#include "Input.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "CircleBody.h"

PhysicsForGamesApp::PhysicsForGamesApp()
{
}

PhysicsForGamesApp::~PhysicsForGamesApp()
{
}

bool PhysicsForGamesApp::startup() 
{
    m_physicsScene.reset(new PhysicsScene());
    m_2dRenderer.reset(new aie::Renderer2D());
    m_font.reset(new aie::Font("./font/consolas.ttf", 16));

    initialiseTutorial1();

    return true;
}

void PhysicsForGamesApp::initialiseTutorial1()
{
    m_physicsScene->SetGravity(-9.8f);
    m_physicsScene->SetTimeStep(1.0f / 60.0f);

    // add four balls to our simulation
    for (int i = 1; i < 5; ++i)
    {
        std::unique_ptr<CircleBody> newBall(new CircleBody(
            glm::vec2(-10 * i, 0),
            glm::vec2(0, 0),
            3.0f, 40,
            glm::vec4(1, 0, 0, 1)));

        m_physicsScene->AddActor(std::move(newBall));
    }
}

void PhysicsForGamesApp::shutdown() 
{
}

void PhysicsForGamesApp::update(float deltaTime) 
{
    aie::Input* input = aie::Input::getInstance();

    m_physicsScene->Update();

    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
    {
        quit();
    }
}

void PhysicsForGamesApp::draw() 
{
    clearScreen();

    m_2dRenderer->begin();

    m_physicsScene->Draw(m_2dRenderer.get());

    m_2dRenderer->drawText(m_font.get(), "Press ESC to quit", 0, 0);

    m_2dRenderer->end();
}