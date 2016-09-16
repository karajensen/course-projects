////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsForGamesApp.h"
#include "Font.h"
#include "Input.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

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

    return true;
}

void PhysicsForGamesApp::shutdown() 
{
}

void PhysicsForGamesApp::update(float deltaTime) 
{
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

    m_2dRenderer->drawText(m_font.get(), "Press ESC to quit", 0, 0);

    m_2dRenderer->end();
}