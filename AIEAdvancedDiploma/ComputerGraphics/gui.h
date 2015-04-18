////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <functional>

struct CTwBar;
struct SceneData;
class Scene;
class Input;
class Tweaker;
class Timer;
class Camera;

/**
* Opens up a tweak bar for manipulating the scene
*/
class Gui
{
public:

    /**
    * Constructor
    * @param scene The scene to manipulate
    * @param camera Allows modifying the camera
    * @param input Allows adding key callbacks
    * @param timer Allows viewing the application times
    * @param wireframe Callback to toggle the wireframe
    */
    Gui(Scene& scene, 
        Camera& camera, 
        Input& input,
        Timer& timer,
        std::function<void(void)> wireframe);

    /**
    * Destructor
    */
    ~Gui();

    /**
    * Renders the tweak bar
    */
    void Render();

    /**
    * Updates the input to the tweak bar
    */
    void Update(const Input& input);

private:

    /**
    * Prevent copying
    */
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;

    /**
    * Refills the tweak bar
    */
    void FillTweakBar();

    Camera& m_camera;                      ///< Allows modifying the view
    Scene& m_scene;                        ///< The scene to manipulates
    SceneData& m_data;                     ///< Data for manipulating the scene
    CTwBar* m_tweakbar = nullptr;          ///< Tweak bar for manipulating the scene
    Timer& m_timer;                        ///< Allows viewing the application times
    std::unique_ptr<Tweaker> m_tweaker;    ///< Helper for modifying the tweak bar
    std::function<void(void)> m_wireframe; ///< Callback for toggle wireframe
    int m_selectedMesh = 0;                ///< Currently selected mesh to tweak
    int m_selectedEmitter = 0;             ///< Currently selected emitter to tweak
    int m_selectedTerrain = 0;             ///< Currently selected terrain to tweak
    int m_selectedWater = 0;               ///< Currently selected water to tweak
    int m_selectedLight = 0;               ///< Currently selected light to tweak
    int m_selectedTexture = 0;             ///< Currently selected procedural texture
};                     