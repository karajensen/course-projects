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
}

class Tweaker;
class PhysicsScene;
class TutorialCreator;
class Input;

class PhysicsForGamesApp : public aie::Application 
{
public:

    /**
    * Constructor
    */
    PhysicsForGamesApp();

    /**
    * Destructor
    */
    virtual ~PhysicsForGamesApp();
    
    /**
    * Starts the application
    */
    void start();

    /**
    * Called on start of application
    * @see aie::Application::startup
    */
    virtual bool startup();

    /**
    * Called on shutdown of application
    * @see aie::Application::shutdown
    */
    virtual void shutdown();

    /**
    * Called on update of application
    * @see aie::Application::update
    * @param deltaTime The time passed between frames rendered
    */
    virtual void update(float deltaTime);

    /**
    * Called on render of application
    * @see aie::Application::draw
    */
    virtual void draw();

private:

    /**
    * Prevent Copying
    */
    PhysicsForGamesApp(const PhysicsForGamesApp&) = delete;
    PhysicsForGamesApp& operator=(const PhysicsForGamesApp&) = delete;

    glm::ivec2 m_size;                              ///< Size of the window
    std::unique_ptr<PhysicsScene> m_physicsScene;   ///< Scene for manipulating physics actors
    std::unique_ptr<TutorialCreator> m_tutorials;   ///< Creates different tutorial scenes
    std::unique_ptr<Tweaker> m_tweaker;             ///< Tweak bar for manipulating the scene
    std::unique_ptr<aie::Renderer2D> m_2dRenderer;  ///< Renderer for drawing 2D objects
    std::unique_ptr<Input> m_input;                 ///< For querying user input
};