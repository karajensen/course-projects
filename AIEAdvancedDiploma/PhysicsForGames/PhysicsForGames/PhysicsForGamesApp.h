////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Application.h"
#include <memory>

namespace aie
{
    class Renderer2D;
    class Font;
}

class PhysicsScene;

class PhysicsForGamesApp : public aie::Application 
{
public:

    PhysicsForGamesApp();
    virtual ~PhysicsForGamesApp();

    virtual bool startup();
    virtual void shutdown();

    virtual void update(float deltaTime);
    virtual void draw();

protected:

    std::unique_ptr<PhysicsScene> m_physicsScene;
    std::unique_ptr<aie::Renderer2D> m_2dRenderer;
    std::unique_ptr<aie::Font> m_font;
};