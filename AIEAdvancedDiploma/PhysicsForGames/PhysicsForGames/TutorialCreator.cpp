////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "PhysicsScene.h"
#include "Input.h"
#include "Tweaker.h"

#include "Tutorial1.h"

TutorialCreator::TutorialCreator(aie::Input& input, PhysicsScene& scene, const glm::ivec2& size)
    : m_scene(scene)
    , m_size(size)
    , m_input(input)
{
}

Tutorial TutorialCreator::Selected() const
{
    return m_currentTutorial;
}

void TutorialCreator::AddToTweaker(Tweaker& tweaker)
{
    tweaker.SetGroup("Tutorials");

    std::vector<const char*> tutorials;
    for (int i = 0; i < Tutorial::TUTORIAL_COUNT; ++i)
    {
        tutorials.push_back(GetName(i));
    }

    tweaker.AddEnumEntry("Selected Tutorial",
        [this]()->int { return m_currentTutorial;  },
        [this, &tweaker](int value) { Create(tweaker, value);  },
        tutorials);

    tweaker.AddButton("Reset Tutorial", [this, &tweaker]()
    {
        Create(tweaker, m_currentTutorial);
    });
}

const char* TutorialCreator::GetName(int tutorial) const
{
    switch (tutorial)
    {
    case TUTORIAL_NONE:
        return "No Tutorial";
    case TUTORIAL_1:
        return "Tutorial 1";
    default:
        return "Unknown";
    }
}

void TutorialCreator::Create(Tweaker& tweaker, int tutorial)
{
    tweaker.Reset();
    m_scene.Reset();
    m_flts.clear();
    m_ints.clear();

    tweaker.SetGroup("Tutorials");

    switch (tutorial)
    {
    case TUTORIAL_NONE:
        break;
    case TUTORIAL_1:
        CreateTutorial1(tweaker);
        break;
    default:
        tutorial = TUTORIAL_NONE;
        LogError("Unsupported tutorial");
    }

    m_currentTutorial = static_cast<Tutorial>(tutorial);
}