////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Input.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Input.h"
#include "../bootstrap/Input.h"
#include "AntTweakBar.h"
#include "Tweaker.h"
#include "glm/glm.hpp"

Input::Input(const glm::ivec2& size, aie::Input& input)
    : m_input(input)
    , m_size(size)
{
    m_mousePosition.x = m_input.getMouseX();
    m_mousePosition.y = m_input.getMouseY();

    m_keyState[aie::INPUT_KEY_0] = KeyState('0');
    m_keyState[aie::INPUT_KEY_1] = KeyState('1');
    m_keyState[aie::INPUT_KEY_2] = KeyState('2');
    m_keyState[aie::INPUT_KEY_3] = KeyState('3');
    m_keyState[aie::INPUT_KEY_4] = KeyState('4');
    m_keyState[aie::INPUT_KEY_5] = KeyState('5');
    m_keyState[aie::INPUT_KEY_6] = KeyState('6');
    m_keyState[aie::INPUT_KEY_7] = KeyState('7');
    m_keyState[aie::INPUT_KEY_8] = KeyState('8');
    m_keyState[aie::INPUT_KEY_9] = KeyState('9');
    m_keyState[aie::INPUT_KEY_PERIOD] = KeyState('.');
    m_keyState[aie::INPUT_KEY_MINUS] = KeyState('-');
    m_keyState[aie::INPUT_KEY_ENTER] = KeyState(TW_KEY_RETURN);
    m_keyState[aie::INPUT_KEY_LEFT] = KeyState(TW_KEY_LEFT);
    m_keyState[aie::INPUT_KEY_RIGHT] = KeyState(TW_KEY_RIGHT);
    m_keyState[aie::INPUT_KEY_TAB] = KeyState(TW_KEY_TAB);
    m_keyState[aie::INPUT_KEY_END] = KeyState(TW_KEY_END);
    m_keyState[aie::INPUT_KEY_HOME] = KeyState(TW_KEY_HOME);
    m_keyState[aie::INPUT_KEY_BACKSPACE] = KeyState(TW_KEY_BACKSPACE);
}

Input::~Input()
{
}

void Input::AddToTweaker(Tweaker& tweaker)
{
    //tweaker.AddStrEntry("Mouse Down", [this]() { return IsMouseDown() ? "Yes" : "No"; });
    //tweaker.AddIntEntry("Mouse X", [this]() { return X(); });
    //tweaker.AddIntEntry("Mouse Y", [this]() { return Y(); });
}

glm::vec2 Input::Convert(const glm::ivec2& mouse)
{
    // Screen coordinates start at top left corner
    // World coordinates start at bottom left corner
    return glm::vec2(mouse.x, m_size.y - mouse.y);
}

void Input::Update()
{
    m_mousePressed = false;
    m_mouseReleased = false;
    m_mouseMoved = false;

    const int x = m_input.getMouseX();
    const int y = m_input.getMouseY();
    const bool hasMouseMoved = x != m_mousePosition.x || y != m_mousePosition.y;

    if (x != m_mousePosition.x || y != m_mousePosition.y)
    {
        m_mouseMoved = true;
    }

    const bool isMouseDown = m_input.wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT);
    if(isMouseDown && !m_mouseDown)
    {
        m_mousePressed = true;
        m_mouseDownPosition.x = x;
        m_mouseDownPosition.y = y;
    }
    else if (!isMouseDown && m_mouseDown)
    {
        m_mouseReleased = true;
        m_mouseUpPosition.x = x;
        m_mouseUpPosition.y = y;
    }

    m_mouseDown = isMouseDown;
    m_mousePosition.x = x;
    m_mousePosition.y = y;

    for (auto& key : m_keyState)
    {
        key.second.pressed = false;

        const bool keyPressed = m_input.wasKeyPressed(key.first);
        if (keyPressed && !key.second.down)
        {
            key.second.pressed = true;
        }
        key.second.down = keyPressed;
    }
}

const std::map<int, Input::KeyState>& Input::GetKeys() const
{
    return m_keyState;
}

bool Input::WasMousePressed() const
{
    return m_mousePressed;
}

bool Input::WasMouseReleased() const
{
    return m_mouseReleased;
}

bool Input::HasMouseMoved() const
{
    return m_mouseMoved;
}

int Input::X() const
{
    return m_input.getMouseX();
}

int Input::Y() const
{
    return m_input.getMouseY();
}

const glm::ivec2& Input::MouseUpPosition() const
{
    return m_mouseUpPosition;
}

const glm::ivec2& Input::MouseDownPosition() const
{
    return m_mouseDownPosition;
}

bool Input::IsMouseDown() const
{
    return m_mouseDown;
}

const glm::ivec2& Input::MousePosition() const
{
    return m_mousePosition;
}