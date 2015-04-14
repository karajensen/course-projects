////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "scene.h"
#include "sceneData.h"
#include "renderdata.h"
#include "tweaker.h"
#include "input.h"
#include "camera.h"
#include "timer.h"
#include <sstream>

Gui::Gui(Scene& scene, 
         Camera& camera,
         Input& input,
         const Timer& timer,
         std::function<void(void)> wireframe) :

    m_scene(scene),
    m_camera(camera),
    m_data(scene.GetData()),
    m_wireframe(wireframe),
    m_timer(timer)
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string barname = "GraphicsTweaker";
    m_tweakbar = TwNewBar(barname.c_str());
    m_tweaker = std::make_unique<Tweaker>(m_tweakbar);
    
    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Graphics Tweaker' " 
        << "position='" << border << " " << border << "' "
        << "size='250 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());

    FillTweakBar();

    auto AddKeyCallback = [&input](unsigned int key, unsigned int code)
    {
        input.AddCallback(key, false, [code]()
        { 
            TwKeyPressed(code, 0);
        });
    };

    // Keys required for modifying entries in the tweak bar
    AddKeyCallback(GLFW_KEY_0, '0');
    AddKeyCallback(GLFW_KEY_1, '1');
    AddKeyCallback(GLFW_KEY_2, '2');
    AddKeyCallback(GLFW_KEY_3, '3');
    AddKeyCallback(GLFW_KEY_4, '4');
    AddKeyCallback(GLFW_KEY_5, '5');
    AddKeyCallback(GLFW_KEY_6, '6');
    AddKeyCallback(GLFW_KEY_7, '7');
    AddKeyCallback(GLFW_KEY_8, '8');
    AddKeyCallback(GLFW_KEY_9, '9');
    AddKeyCallback(GLFW_KEY_PERIOD, '.');
    AddKeyCallback(GLFW_KEY_ENTER, TW_KEY_RETURN);
    AddKeyCallback(GLFW_KEY_BACKSPACE, TW_KEY_BACKSPACE);
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

void Gui::FillTweakBar()
{
    TwRemoveAllVars(m_tweakbar);
    m_tweaker->ClearEntries();

    m_tweaker->SetGroup("Scene");
    m_tweaker->AddEntry("Render Pass", [this](){ return m_data.post->GetPostMap(); });
    m_tweaker->AddEntry("Frames Per Second", [this](){ return std::to_string(m_timer.GetFPS()); });
    m_tweaker->AddButton("Toggle Wireframe", m_wireframe);
    m_tweaker->AddButton("Reload Scene", [this](){ m_scene.Reload(); });
    m_tweaker->AddButton("Save Textures", [this](){ m_scene.SaveTextures(); });

    m_tweaker->SetGroup("Post Processing");
    m_data.post->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Meshes");
    m_tweaker->AddEntry("Selected Mesh", 
        [this](){ return m_selectedMesh; },
        [this](const int value){ m_selectedMesh = value; FillTweakBar(); }, 
        m_data.meshes.size()-1);
    m_data.meshes[m_selectedMesh]->AddToTweaker(*m_tweaker);
    
    m_tweaker->SetGroup("Lights");
    m_tweaker->AddEntry("Selected Light", 
        [this](){ return m_selectedLight; },
        [this](const int value){ m_selectedLight = value; FillTweakBar(); }, 
        m_data.lights.size()-1);
    m_data.lights[m_selectedLight]->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Terrain");
    m_tweaker->AddEntry("Selected Terrain", 
        [this](){ return m_selectedTerrain; },
        [this](const int value){ m_selectedTerrain = value; FillTweakBar(); }, 
        m_data.terrain.size()-1);
    m_data.terrain[m_selectedTerrain]->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Water");
    m_tweaker->AddEntry("Selected Water", 
        [this](){ return m_selectedWater; },
        [this](const int value){ m_selectedWater = value; FillTweakBar(); }, 
        m_data.water.size()-1);
    m_data.water[m_selectedWater]->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Emitters");
    m_tweaker->AddEntry("Selected Emitter", 
        [this](){ return m_selectedEmitter; },
        [this](const int value){ m_selectedEmitter = value; FillTweakBar(); }, 
        m_data.emitters.size()-1);
    m_data.emitters[m_selectedEmitter]->AddToTweaker(*m_tweaker);
    
    m_tweaker->SetGroup("Textures");
    m_tweaker->AddEntry("Selected Texture", 
        [this](){ return m_selectedTexture; },
        [this](const int value){ m_selectedTexture = value; FillTweakBar(); }, 
        m_data.proceduralTextures.size()-1);
    m_data.textures[m_data.proceduralTextures[m_selectedTexture]]->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Caustics");
    m_data.animation[Animation::ID_CAUSTICS]->AddToTweaker(*m_tweaker);

    m_tweaker->SetGroup("Camera");
    m_camera.AddToTweaker(*m_tweaker);
}