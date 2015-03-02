////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - input.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "input.h"
#include "glfw/glfw3.h"

Input::Input(GLFWwindow& window) :
    m_window(window)
{
}

void Input::UpdateMouse()
{
    m_mousePressed = glfwGetMouseButton(
        &m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    double xPosition = 0.0, yPosition = 0.0;
    glfwGetCursorPos(&m_window, &xPosition, &yPosition);

    const int x = static_cast<int>(xPosition);
    const int y = static_cast<int>(yPosition);

    m_mouseDirection.x = static_cast<float>(m_mouseX) - x;
    m_mouseDirection.y = static_cast<float>(m_mouseY) - y;

    const float length = glm::length(m_mouseDirection);
    if (length != 0.0f)
    {
        m_mouseDirection /= length;
    }

    m_mouseX = x;
    m_mouseY = y;
}

void Input::Update()
{
    UpdateMouse();

    for(KeyMap::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
    {
        const bool pressed = glfwGetKey(&m_window, it->first) == GLFW_PRESS;
        UpdateKey(pressed, it->second.state);

        const bool keyDown = it->second.continuous ? 
            IsKeyDownContinous(it->second.state) : 
            IsKeyDown(it->second.state);

        if(keyDown && it->second.onKeyFn != nullptr)
        {
            it->second.onKeyFn();
        }
    }
}

void Input::UpdateKey(bool pressed, unsigned int& state)
{
    if(pressed && (state & KEY_DOWN) != KEY_DOWN)
    {
        //save key as pressed
        state |= KEY_DOWN;
    }
    else if(!pressed && (state & KEY_DOWN) == KEY_DOWN)
    {
        //save key as not pressed
        state &= ~KEY_DOWN;
    }
}

bool Input::IsKeyDownContinous(unsigned int state) const
{
    return (state & KEY_DOWN) == KEY_DOWN;
}

bool Input::IsKeyDown(unsigned int& state)
{
    if(((state & KEY_DOWN) == KEY_DOWN) &&
       ((state & KEY_QUERIED) != KEY_QUERIED))
    {
        // Add user has queried for key to prevent constant queries
        state |= KEY_QUERIED;
        return true;
    }
    else if((state & KEY_DOWN) != KEY_DOWN)
    {
        // Key is lifted, allow user to query again
        state &= ~KEY_QUERIED;
    }
    return false;
}

void Input::AddCallback(unsigned int key, bool onContinous, Input::KeyFn onKeyFn)
{
    m_keys[key].state = KEY_NONE;
    m_keys[key].continuous = onContinous;
    m_keys[key].onKeyFn = onKeyFn;
}

const glm::vec2& Input::GetMouseDirection() const
{
    return m_mouseDirection;
}

bool Input::IsMousePressed() const
{
    return m_mousePressed;
}