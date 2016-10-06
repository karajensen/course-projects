////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsForGamesApp.h"
#include "Font.h"
#include "Input.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "TutorialCreator.h"
#include "Gui.h"

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
    auto& input = *aie::Input::getInstance();

    m_2dRenderer.reset(new aie::Renderer2D());
    m_physicsScene.reset(new PhysicsScene(*m_2dRenderer));
    m_tutorials.reset(new TutorialCreator(input, *m_physicsScene, m_size));
    m_gui.reset(new Gui(*m_physicsScene, *m_tutorials, input, m_size));

    // Always create the latest tutorial
    m_tutorials->Create(m_gui->GetTweaker(), Tutorial::TUTORIAL_COUNT-1);

    return true;
}

void PhysicsForGamesApp::shutdown() 
{
}

void PhysicsForGamesApp::update(float deltaTime) 
{
    m_physicsScene->Update();
    m_gui->Update();

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

    m_gui->Render();
}