////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Gui.h"
#include "Tweaker.h"
#include "TutorialCreator.h"
#include "Input.h"
#include <sstream>

Gui::Gui(TutorialCreator& tutorials,
         aie::Input& input,
         const glm::ivec2& size)
    : m_tutorials(tutorials)
    , m_input(input)
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(size.x, size.y);

    const std::string barname = "GamesForPhysics";
    m_tweakbar = TwNewBar(barname.c_str());
    m_tweaker = std::make_unique<Tweaker>(m_tweakbar);
    
    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Games For Physics' " 
        << "position='" << border << " " << border << "' "
        << "size='250 450' "
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());

    auto resetTweakBar = [this]()
    {
        m_tutorials.AddToTweaker(GetTweaker());
    };

    m_tweaker->AddResetFn(resetTweakBar);

    m_mousePosition.x = m_input.getMouseX();
    m_mousePosition.y = m_input.getMouseY();
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
    const bool isMouseDown = m_input.wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT);

    if (x != m_mousePosition.x || y != m_mousePosition.y)
    {
        TwMouseMotion(x, y);
    }
    
    if(isMouseDown && !m_mouseDown)
    {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
    }
    else if (!isMouseDown && m_mouseDown)
    {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
    }

    m_mouseDown = isMouseDown;
    m_mousePosition.x = x;
    m_mousePosition.y = y;
}

void Gui::UpdateKeys()
{
    auto UpdateKey = [this](unsigned int key, unsigned int code)
    {
        if (m_input.wasKeyPressed(key))
        {
            TwKeyPressed(code, 0);
        }
    };

    UpdateKey(aie::INPUT_KEY_0, '0');
    UpdateKey(aie::INPUT_KEY_1, '1');
    UpdateKey(aie::INPUT_KEY_2, '2');
    UpdateKey(aie::INPUT_KEY_3, '3');
    UpdateKey(aie::INPUT_KEY_4, '4');
    UpdateKey(aie::INPUT_KEY_5, '5');
    UpdateKey(aie::INPUT_KEY_6, '6');
    UpdateKey(aie::INPUT_KEY_7, '7');
    UpdateKey(aie::INPUT_KEY_8, '8');
    UpdateKey(aie::INPUT_KEY_9, '9');
    UpdateKey(aie::INPUT_KEY_PERIOD, '.');
    UpdateKey(aie::INPUT_KEY_MINUS, '-');
    UpdateKey(aie::INPUT_KEY_ENTER, TW_KEY_RETURN);
    UpdateKey(aie::INPUT_KEY_LEFT, TW_KEY_LEFT);
    UpdateKey(aie::INPUT_KEY_RIGHT, TW_KEY_RIGHT);
    UpdateKey(aie::INPUT_KEY_TAB, TW_KEY_TAB);
    UpdateKey(aie::INPUT_KEY_END, TW_KEY_END);
    UpdateKey(aie::INPUT_KEY_HOME, TW_KEY_HOME);
    UpdateKey(aie::INPUT_KEY_BACKSPACE, TW_KEY_BACKSPACE);
}