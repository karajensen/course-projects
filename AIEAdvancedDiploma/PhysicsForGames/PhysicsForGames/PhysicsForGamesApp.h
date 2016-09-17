////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsForGamesApp.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Application.h"
#include "glm/vec2.hpp"
#include <memory>

namespace aie
{
    class Renderer2D;
    class Font;
}

class Gui;
class PhysicsScene;
class TutorialCreator;

class PhysicsForGamesApp : public aie::Application 
{
public:
    PhysicsForGamesApp();
    virtual ~PhysicsForGamesApp();
    
    void start();
    virtual bool startup();
    virtual void shutdown();
    virtual void update(float deltaTime);
    virtual void draw();

private:
    PhysicsForGamesApp(const PhysicsForGamesApp&) = delete;
    PhysicsForGamesApp& operator=(const PhysicsForGamesApp&) = delete;

    glm::ivec2 m_size;
    std::unique_ptr<PhysicsScene> m_physicsScene;
    std::unique_ptr<TutorialCreator> m_tutorials;
    std::unique_ptr<Gui> m_gui;
    std::unique_ptr<aie::Renderer2D> m_2dRenderer;
    std::unique_ptr<aie::Font> m_font;
};