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
         Input& input,
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
        m_input.AddToTweaker(GetTweaker());
        m_tutorials.AddToTweaker(GetTweaker());
    };

    m_tweaker->AddResetFn(resetTweakBar);
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

    if (m_input.HasMouseMoved())
    {
        TwMouseMotion(m_input.X(), m_input.Y());
    }

    if(m_input.WasMousePressed())
    {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
    }
    
    if (m_input.WasMouseReleased())
    {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
    }

    for (auto& key : m_input.GetKeys())
    {
        if (key.second.pressed)
        {
            TwKeyPressed(key.second.code, 0);
        }
    }
}