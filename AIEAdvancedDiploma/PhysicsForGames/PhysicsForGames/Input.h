////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Input.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include "glm/vec2.hpp"

namespace aie
{
    class Input;
}

class Tweaker;

/**
* Manages input from the aie input class
*/
class Input
{
public:

    /**
    * Constructor
    * @param size The size of the window
    * @param input For querying user input
    */
    Input(const glm::ivec2& size, aie::Input& input);

    /**
    * Destructor
    */
    ~Input();

    /**
    * Updates the cached input
    */
    void Update();

    /**
    * Adds to the tweakbar
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * @return if the mouse was pressed last tick
    */
    bool WasMousePressed() const;

    /**
    * @return if the mouse was released last tick
    */
    bool WasMouseReleased() const;

    /**
    * @return if the mouse has moved last tick
    */
    bool HasMouseMoved() const;

    /**
    * @return the x position of the mouse
    */
    int X() const;

    /**
    * @return the y position of the mouse
    */
    int Y() const;

    /**
    * @return the current mouse position
    */
    const glm::ivec2& MousePosition() const;

    /**
    * @return the position where the user last pressed the mouse
    */
    const glm::ivec2& MouseDownPosition() const;

    /**
    * @return the position where the user last released the mouse
    */
    const glm::ivec2& MouseUpPosition() const;

    /**
    * @return if the mouse is currently pressed
    */
    bool IsMouseDown() const;

    /**
    * Converts mouse coordinates to world coordinates
    */
    glm::vec2 Convert(const glm::ivec2& mouse);

    /**
    * Key state information
    */
    struct KeyState
    {
        KeyState() = default;
        KeyState(int c) :
            code(c)
        {
        }

        bool pressed = false;   ///< Whether the key was pressed last tick
        bool down = false;      ///< Whether the key is currently down
        int code = 0;           ///< User key code
    };

    /**
    * @return the key states
    */
    const std::map<int, KeyState>& GetKeys() const;

private:

    /**
    * Prevent Copying
    */
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    const glm::ivec2& m_size;               ///< The size of the window
    aie::Input& m_input;                    ///< For querying user input
    glm::ivec2 m_mousePosition;             ///< Last position of the mouse
    glm::ivec2 m_mouseDownPosition;         ///< Last position where user pressed the mouse
    glm::ivec2 m_mouseUpPosition;           ///< Last position where user release the mouse
    bool m_mousePressed = false;            ///< Whether mouse pressed last tick
    bool m_mouseReleased = false;           ///< Whether mouse released last tick
    bool m_mouseMoved = false;              ///< Whether the mouse moved last tick
    bool m_mouseDown = false;               ///< Whether mouse is currently down
    std::map<int, KeyState> m_keyState;     ///< Last press state of the keys
};                     