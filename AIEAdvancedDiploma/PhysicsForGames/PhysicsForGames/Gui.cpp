////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Gui.h"
#include "Tweaker.h"
#include "TutorialCreator.h"
#include "PhysicsScene.h"
#include "Input.h"
#include <sstream>

Gui::Gui(PhysicsScene& scene,
         TutorialCreator& tutorials,
         aie::Input& input,
         const glm::ivec2& size)
    : m_tutorials(tutorials)
    , m_input(input)
    , m_scene(scene)
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(size.x, size.y);

    const std::string barname = "GamesForPhysics";
    m_tweakbar = TwNewBar(barname.c_str());
    m_tweaker = std::make_unique<Tweaker>(m_tweakbar);
    
    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Physics For Games' " 
        << "position='" << border << " " << border << "' "
        << "size='250 450' "
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());

    auto resetTweakBar = [this]()
    {
        m_scene.AddToTweaker(GetTweaker());
        m_tutorials.AddToTweaker(GetTweaker());
    };

    m_tweaker->AddResetFn(resetTweakBar);

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

Gui::~Gui()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Gui::Render()
{
    TwDraw();
}

Tweaker& Gui::GetTweaker()
{
    return *m_tweaker;
}

void Gui::Update()
{
    m_tweaker->Update();
    UpdateMouse();
    UpdateKeys();
}

void Gui::UpdateMouse()
{
    const int x = m_input.getMouseX();
    const int y = m_input.getMouseY();
    const bool hasMouseMoved = x != m_mousePosition.x || y != m_mousePosition.y;

    if (x != m_mousePosition.x || y != m_mousePosition.y)
    {
        TwMouseMotion(x, y);
    }

    const bool isMouseDown = m_input.wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT);
    if(isMouseDown && !m_mouseState)
    {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
    }
    else if (!isMouseDown && m_mouseState)
    {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
    }

    m_mouseState = isMouseDown;
    m_mousePosition.x = x;
    m_mousePosition.y = y;
}

void Gui::UpdateKeys()
{
    for (auto& key : m_keyState)
    {
        const bool keyPressed = m_input.wasKeyPressed(key.first);
        if (keyPressed && !key.second.down)
        {
            TwKeyPressed(key.second.code, 0);
        }
        key.second.down = keyPressed;
    }
}