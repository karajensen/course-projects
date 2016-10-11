////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TutorialData.h"
#include <functional>
#include <vector>
#include <memory>

class Tweaker;

enum Tutorial
{
    TUTORIAL_NONE,
    TUTORIAL_1,
    TUTORIAL_2,
    TUTORIAL_3,
    TUTORIAL_4,
    TUTORIAL_5,
    TUTORIAL_COUNT
};

/**
* Generates physics bodies for a tutorial scene
*/
class TutorialCreator
{
public:

    /**
    * Constructor
    * @param input For querying user input
    * @param scene Physics scene manager
    * @param tweaker The tweak bar wrapper
    * @param size Window dimensions
    */
    TutorialCreator(Input& input, 
                    PhysicsScene& scene, 
                    Tweaker& tweaker,
                    const glm::ivec2& size);

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
    void Create(int tutorial);

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
    * Create the scene for each tutorial
    */
    void CreateTutorial1();
    void CreateTutorial2();
    void CreateTutorial3();
    void CreateTutorial4();
    void CreateTutorial5();

    Tutorial m_currentTutorial = TUTORIAL_NONE;           ///< Currently selected tutorial
    std::function<void(void)> m_resetTweaker = nullptr;   ///< Callback to reset the tweak bar
    std::unique_ptr<TutorialTweaker> m_tweaker;           ///< Tweak bar wrapper for tutorials
    std::unique_ptr<TutorialData> m_data;                 ///< Data for running tutorials

    typedef void(TutorialCreator::*CreateFn)();
    std::vector<CreateFn> m_createTutorial;               ///< Callback for creating a tutorial
    std::vector<const char*> m_tutorialNames;             ///< Names for each tutorial
};