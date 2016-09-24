////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <map>
#include <functional>
#include "glm/vec2.hpp"

namespace aie
{
    class Input;
}

struct CTwBar;
class Tweaker;
class TutorialCreator;
class PhysicsScene;

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
        aie::Input& input,
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

    /**
    * Updates the cachedmouse state
    */
    void UpdateMouse();

    /**
    * Updates the cached keys state
    */
    void UpdateKeys();

    struct KeyState
    {
        KeyState() = default;
        KeyState(int c) :
            code(c)
        {
        }

        bool down = false;
        int code = 0;
    };

    aie::Input& m_input;                    ///< For querying user input
    CTwBar* m_tweakbar = nullptr;           ///< Internal tweak bar
    std::unique_ptr<Tweaker> m_tweaker;     ///< Wrapper for adding parameters to the tweak bar
    TutorialCreator& m_tutorials;           ///< The creator for switching tutorial scenes
    PhysicsScene& m_scene;                  ///< Physics scene manager
    glm::ivec2 m_mousePosition;             ///< Last position of the mouse
    bool m_mouseState = false;              ///< Last click state of the mouse
    std::map<int, KeyState> m_keyState;     ///< Last press state of the keys
};                     