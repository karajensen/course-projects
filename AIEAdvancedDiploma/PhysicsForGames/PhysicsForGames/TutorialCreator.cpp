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

TutorialCreator::TutorialCreator(Input& input, PhysicsScene& scene, const glm::ivec2& size)
{
    m_data.input = &input;
    m_data.scene = &scene;
    m_data.size = size;

    m_createTutorial = 
    {
        nullptr,
        &TutorialCreator::CreateTutorial1,
        &TutorialCreator::CreateTutorial2,
        &TutorialCreator::CreateTutorial3,
        &TutorialCreator::CreateTutorial4
    };

    m_tutorialNames = 
    {
        "No Tutorial",
        "Tutorial 01",
        "Tutorial 02",
        "Tutorial 03",
        "Tutorial 04"
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
    if (!m_tweaker)
    {
        using namespace std::placeholders;
        m_tweaker.reset(new TutorialTweaker(tweaker,
            std::bind(&TutorialCreator::SetInt, this, _1, _2),
            std::bind(&TutorialCreator::SetFlt, this, _1, _2),
            std::bind(&TutorialCreator::GetInt, this, _1),
            std::bind(&TutorialCreator::GetFlt, this, _1)));
        m_data.tweaker = m_tweaker.get();
    }

    tweaker.Reset();
    m_data.scene->Reset();
    m_data.flts.clear();
    m_data.ints.clear();

    if (tutorial != TUTORIAL_NONE)
    {
        (this->*m_createTutorial.at(tutorial))();
    }

    m_currentTutorial = static_cast<Tutorial>(tutorial);
}

void TutorialCreator::SetInt(const char* name, int value)
{
    m_data.ints.at(name) = value;
}

void TutorialCreator::SetFlt(const char* name, float value)
{
    m_data.flts.at(name) = value;
}

int TutorialCreator::GetInt(const char* name) const
{
    return m_data.ints.at(name);
}

float TutorialCreator::GetFlt(const char* name) const
{
    return m_data.flts.at(name);
}
