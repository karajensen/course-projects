////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <map>
#include <functional>
#include <vector>

class PhysicsScene;
class Tweaker;

namespace aie
{
    class Input;
}

enum Tutorial
{
    TUTORIAL_NONE,
    TUTORIAL_1,
    TUTORIAL_2,
    TUTORIAL_3,
    TUTORIAL_COUNT
};

class TutorialCreator
{
public:

    /**
    * Constructor
    * @param input For querying user input
    * @param scene Physics scene manager
    * @param size Window dimensions
    */
    TutorialCreator(aie::Input& input, PhysicsScene& scene, const glm::ivec2& size);

    /**
    * Creates a new tutorial scene
    * @param tweaker The tweak bar wrapper
    * @Param tutorial The tutorial to create
    * @note an invalid tutorial will create an empty scene
    */
    void Create(Tweaker& tweaker, int tutorial);

    /**
    * @return the name of the turorial
    */
    const char* GetName(int tutorial) const;

    /**
    * @return the current selected tutorial
    */
    Tutorial Selected() const;

    /**
    * Adds the tutorial creator to the tweakbar
    */
    void AddToTweaker(Tweaker& tweaker);

private:

    /**
    * Prevent Copying
    */
    TutorialCreator(const TutorialCreator&) = delete;
    TutorialCreator& operator=(const TutorialCreator&) = delete;

    /**
    * Adds a tweakable float value to the tweak bar
    * @param tweaker The tweak bar to add to
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param precision The number of decimal points to show
    * @param onSet Callback to use when value is set
    */
    void AddTweakableFlt(Tweaker& tweaker, 
                         const char* name, 
                         const char* label,
                         float step,
                         int precision,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable int value to the tweak bar
    * @param tweaker The tweak bar to add to
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakableInt(Tweaker& tweaker, 
                         const char* name, 
                         const char* label,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable float value to the tweak bar
    * @param tweaker The tweak bar to add to
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param precision The number of decimal points to show
    * @param min/max The min and max values to go to
    * @param onSet Callback to use when value is set
    */
    void AddTweakableFlt(Tweaker& tweaker, 
                         const char* name, 
                         const char* label,
                         float step,
                         float min,
                         float max,
                         int precision,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable int value to the tweak bar
    * @param tweaker The tweak bar to add to
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param min/max The min and max values to go to
    * @param onSet Callback to use when value is set
    */
    void AddTweakableInt(Tweaker& tweaker, 
                         const char* name, 
                         const char* label,
                         int min,
                         int max,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Create the scene for each tutorial
    */
    void CreateTutorial1(Tweaker& tweaker);
    void CreateTutorial2(Tweaker& tweaker);
    void CreateTutorial3(Tweaker& tweaker);

    aie::Input& m_input;                                  ///< For querying user input
    PhysicsScene& m_scene;                                ///< Physics scene manager
    const glm::ivec2& m_size;                             ///< Size of the window
    Tutorial m_currentTutorial = TUTORIAL_NONE;           ///< Currently selected tutorial
    std::map<const char*, float> m_flts;                  ///< Current tutorial specific parameters
    std::map<const char*, int> m_ints;                    ///< Current tutorial specific parameters
    std::function<void(void)> m_resetTweaker = nullptr;   ///< Callback to reset the tweak bar

    typedef void(TutorialCreator::*CreateFn)(Tweaker&);
    std::vector<CreateFn> m_createTutorial;               ///< Callback for creating a tutorial
    std::vector<const char*> m_tutorialNames;             ///< Tutorial names
};