////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "TutorialTweaker.h"
#include "PhysicsScene.h"
#include "Input.h"
#include "Tweaker.h"
#include "Utilities.h"
#include <algorithm>

TutorialCreator::TutorialCreator(Input& input, 
                                 PhysicsScene& scene, 
                                 Tweaker& tweaker,
                                 const glm::ivec2& size)
{
    m_tweaker.reset(new TutorialTweaker(tweaker));
    m_data.reset(new TutorialData(input, scene, *m_tweaker, size));
    m_tweaker->SetData(m_data.get());
       
    m_createTutorial = 
    {
        nullptr,
        &TutorialCreator::CreateTutorial1,
        &TutorialCreator::CreateTutorial2,
        &TutorialCreator::CreateTutorial3,
        &TutorialCreator::CreateTutorial4,
        &TutorialCreator::CreateTutorial5,
        &TutorialCreator::CreateTutorial6
    };

    m_tutorialNames = 
    {
        "No Tutorial",
        "Tutorial 1",
        "Tutorial 2",
        "Tutorial 3",
        "Tutorial 4",
        "Tutorial 5",
        "Tutorial 6"
    };
}

TutorialCreator::~TutorialCreator()
{

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
        [this, &tweaker](int value) { Create(value);  },
        tutorials);

    tweaker.AddButton("Reset Tutorial", [this, &tweaker]()
    {
        Create(m_currentTutorial);
    });
}

const char* TutorialCreator::GetName(int tutorial) const
{
    return m_tutorialNames.at(tutorial);
}

void TutorialCreator::Create(int tutorial)
{
    m_data->Reset();

    if (tutorial != TUTORIAL_NONE)
    {
        (this->*m_createTutorial.at(tutorial))();
    }

    m_currentTutorial = static_cast<Tutorial>(tutorial);
}
