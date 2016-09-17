////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
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

    aie::Input& m_input;
    CTwBar* m_tweakbar = nullptr;
    std::unique_ptr<Tweaker> m_tweaker;
    TutorialCreator& m_tutorials;
    glm::ivec2 m_mousePosition;
    bool m_mouseDown = false;
};                     