////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <map>
#include <functional>
#include "glm/vec2.hpp"

struct CTwBar;
class Tweaker;
class TutorialCreator;
class PhysicsScene;
class Input;

class Gui
{
public:

    /**
    * Constructor
    * @param scene Physics scene manager
    * @param tutorials The creator for switching tutorial scenes
    * @param input For querying user input
    * @param size The window size
    */
    Gui(PhysicsScene& scene,
        TutorialCreator& tutorials,
        Input& input,
        const glm::ivec2& size);

    /**
    * Destructor
    */
    ~Gui();

    /**
    * Renders the GUI tweak bar
    */
    void Render();

    /**
    * Updates the GUI tweak bar
    */
    void Update();

    /**
    * @return the GUI tweak bar
    */
    Tweaker& GetTweaker();

private:

    /**
    * Prevent Copying
    */
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;

    CTwBar* m_tweakbar = nullptr;           ///< Internal tweak bar
    std::unique_ptr<Tweaker> m_tweaker;     ///< Wrapper for adding parameters to the tweak bar
    TutorialCreator& m_tutorials;           ///< The creator for switching tutorial scenes
    PhysicsScene& m_scene;                  ///< Physics scene manager
    Input& m_input;                         ///< For querying user input
};                     