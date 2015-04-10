////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "scene.h"
#include "sceneData.h"
#include "renderdata.h"
#include "input.h"
#include "anttweakbar/AntTweakBar.h"
#include <sstream>

Gui::Gui(Scene& scene) :
    m_scene(scene),
    m_data(scene.GetData())
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string barname = "GraphicsTweaker";
    m_tweakbar = TwNewBar(barname.c_str());
    
    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Graphics Tweaker' " 
        << "position='" << border << " " << border << "' "
        << "size='200 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());
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
void Gui::Update(const Input& input)
{
    if (input.IsMouseMovedThisTick())
    {
        TwMouseMotion(input.GetMouseX(), input.GetMouseY());
    }
    else if (input.IsLeftMousePressedThisTick())
    {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
    }
    else if (input.IsLeftMouseReleasedThisTick())
    {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
    }
}