////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "PhysicsScene.h"
#include "Input.h"
#include "Tweaker.h"
#include "Utilities.h"
#include <algorithm>

TutorialCreator::TutorialCreator(aie::Input& input, PhysicsScene& scene, const glm::ivec2& size)
    : m_scene(scene)
    , m_size(size)
    , m_input(input)
{
    m_createTutorial = 
    {
        nullptr,
        &TutorialCreator::CreateTutorial1,
        &TutorialCreator::CreateTutorial2,
        &TutorialCreator::CreateTutorial3
    };

    m_tutorialNames =
    {
        "No Tutorial",
        "Tutorial 01",
        "Tutorial 02",
        "Tutorial 03"
    };
}

Tutorial TutorialCreator::Selected() const
{
    return m_currentTutorial;
}

void TutorialCreator::AddToTweaker(Tweaker& tweaker)
{
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
    return m_tutorialNames.at(tutorial);
}

void TutorialCreator::Create(Tweaker& tweaker, int tutorial)
{
    tweaker.Reset();
    m_scene.Reset();
    m_flts.clear();
    m_ints.clear();

    if (tutorial != TUTORIAL_NONE)
    {
        (this->*m_createTutorial.at(tutorial))(tweaker);
    }

    m_currentTutorial = static_cast<Tutorial>(tutorial);
}

void TutorialCreator::AddTweakableFlt(Tweaker& tweaker,
                                      const char* name, 
                                      const char* label, 
                                      float step,
                                      int precision,
                                      std::function<void(void)> onSet)
{
    AddTweakableFlt(tweaker, name, label, step, -FLT_MAX, FLT_MAX, precision, onSet);
}

void TutorialCreator::AddTweakableFlt(Tweaker& tweaker,
                                      const char* name, 
                                      const char* label, 
                                      float step,
                                      float min,
                                      float max,
                                      int precision,
                                      std::function<void(void)> onSet)
{
    assert(m_flts.find(name) != m_flts.end());

    auto setFlt = [this, name, onSet, min, max](float value)
    {
        m_flts.at(name) = std::max(std::min(value, max), min);

        if (onSet)
        {
            onSet();
        }
    };

    tweaker.AddFltEntry(label,
        [this, name]() { return m_flts.at(name); },
        setFlt, step, precision);
}

void TutorialCreator::AddTweakableInt(Tweaker& tweaker,
                                      const char* name, 
                                      const char* label,
                                      std::function<void(void)> onSet)
{
    AddTweakableInt(tweaker, name, label, INT_MIN, INT_MAX, onSet);
}

void TutorialCreator::AddTweakableInt(Tweaker& tweaker,
                                      const char* name, 
                                      const char* label,
                                      int min, 
                                      int max,
                                      std::function<void(void)> onSet)
{
    assert(m_ints.find(name) != m_ints.end());

    auto setInt = [this, name, onSet, min, max](int value)
    {
        m_ints.at(name) = std::max(std::min(value, max), min);

        if (onSet)
        {
            onSet();
        }
    };

    assert(m_ints.find(name) != m_ints.end());
    tweaker.AddIntEntry(label,
        [this, name]() { return m_ints.at(name); },
        setInt, INT_MAX);
}
