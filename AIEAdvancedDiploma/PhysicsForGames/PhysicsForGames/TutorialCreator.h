////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <map>
#include <functional>
#include <vector>
#include <memory>

class PhysicsScene;
class Tweaker;
class TutorialTweaker;
class Input;

enum Tutorial
{
    TUTORIAL_NONE,
    TUTORIAL_1,
    TUTORIAL_2,
    TUTORIAL_3,
    TUTORIAL_4,
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
    TutorialCreator(Input& input, PhysicsScene& scene, const glm::ivec2& size);

    /**
    * Destructor
    */
    ~TutorialCreator();

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
    * Sets tutorial int data
    */
    void SetInt(const char* name, int value);

    /**
    * Sets tutorial float data
    */
    void SetFlt(const char* name, float value);

    /**
    * Gets tutorial int data
    */
    int GetInt(const char* name) const;

    /**
    * Gets tutorial float data
    */
    float GetFlt(const char* name) const;

    /**
    * Create the scene for each tutorial
    */
    void CreateTutorial1();
    void CreateTutorial2();
    void CreateTutorial3();
    void CreateTutorial4();

    Input& m_input;                                       ///< For querying user input
    PhysicsScene& m_scene;                                ///< Physics scene manager
    const glm::ivec2& m_size;                             ///< Size of the window
    Tutorial m_currentTutorial = TUTORIAL_NONE;           ///< Currently selected tutorial
    std::map<const char*, float> m_flts;                  ///< Current tutorial specific parameters
    std::map<const char*, int> m_ints;                    ///< Current tutorial specific parameters
    std::function<void(void)> m_resetTweaker = nullptr;   ///< Callback to reset the tweak bar
    std::unique_ptr<TutorialTweaker> m_tweaker;           ///< Tweak bar wrapper for tutorials

    typedef void(TutorialCreator::*CreateFn)();
    std::vector<CreateFn> m_createTutorial;               ///< Callback for creating a tutorial
    std::vector<const char*> m_tutorialNames;             ///< Names for each tutorial
};