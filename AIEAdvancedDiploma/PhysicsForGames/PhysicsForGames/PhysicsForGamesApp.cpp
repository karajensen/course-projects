////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsForGamesApp.h"
#include "Font.h"
#include "Input.h"
#include "../bootstrap/Input.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "TutorialCreator.h"
#include "Tweaker.h"

PhysicsForGamesApp::PhysicsForGamesApp() :
    m_size(1280, 720)
{
}

PhysicsForGamesApp::~PhysicsForGamesApp()
{
}

void PhysicsForGamesApp::start()
{
    run("AIE", m_size.x, m_size.y, false);
}

bool PhysicsForGamesApp::startup() 
{
    setBackgroundColour(0.2f, 0.2f, 0.2f);
    m_input.reset(new Input(m_size, *aie::Input::getInstance()));
    m_2dRenderer.reset(new aie::Renderer2D());
    m_physicsScene.reset(new PhysicsScene(*m_2dRenderer));
    m_tweaker.reset(new Tweaker(m_size));
    m_tutorials.reset(new TutorialCreator(*m_input, *m_physicsScene, *m_tweaker, m_size));

    m_tweaker->AddResetFn([this]()
    {
        m_physicsScene->AddToTweaker(*m_tweaker);
        m_input->AddToTweaker(*m_tweaker);
        m_tutorials->AddToTweaker(*m_tweaker);
    });
    m_tweaker->Reset();

    // Always create the latest tutorial
    m_tutorials->Create(Tutorial::TUTORIAL_COUNT-1);

    return true;
}

void PhysicsForGamesApp::shutdown() 
{
}

void PhysicsForGamesApp::update(float deltaTime) 
{
    m_input->Update();
    m_physicsScene->Update();
    m_tweaker->Update(*m_input);

    aie::Input* input = aie::Input::getInstance();
    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
    {
        quit();
    }
}

void PhysicsForGamesApp::draw() 
{
    clearScreen();

    m_2dRenderer->begin();

    m_physicsScene->Draw();

    m_2dRenderer->end();

    m_tweaker->Render();
}