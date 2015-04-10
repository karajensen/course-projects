////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

struct CTwBar;
struct SceneData;
class Scene;
class Input;

/**
* Opens up a tweak bar for manipulating the scene
*/
class Gui
{
public:

    /**
    * Constructor
    * @param scene The scene to manipulate
    */
    Gui(Scene& scene);

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

    Scene& m_scene;                  ///< The scene to manipulates
    SceneData& m_data;               ///< Data for manipulating the scene
    CTwBar* m_tweakbar = nullptr;    ///< Tweak bar for manipulating the scene
};                     