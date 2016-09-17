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

class Gui
{
public:
    Gui(TutorialCreator& tutorials,
        aie::Input& input,
        const glm::ivec2& size);
    ~Gui();

    void Render();
    void Update();
    Tweaker& GetTweaker();

private:
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;

    void UpdateMouse();
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

    aie::Input& m_input;
    CTwBar* m_tweakbar = nullptr;
    std::unique_ptr<Tweaker> m_tweaker;
    TutorialCreator& m_tutorials;
    glm::ivec2 m_mousePosition;
    bool m_mouseState = false;
    std::map<int, KeyState> m_keyState;
};                     