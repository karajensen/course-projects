////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialData.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialData.h"
#include "TutorialTweaker.h"
#include "PhysicsScene.h"

TutorialData::TutorialData(::Input& input,
                           PhysicsScene& scene, 
                           TutorialTweaker& tweaker,
                           const glm::ivec2& size)
    : m_input(input)
    , m_scene(scene)
    , m_tweaker(tweaker)
    , m_size(size)
{
}

void TutorialData::SetInt(const char* name, int value)
{
    m_ints.at(name) = value;
}

void TutorialData::SetFlt(const char* name, float value)
{
    m_flts.at(name) = value;
}

int TutorialData::GetInt(const char* name) const
{
    return m_ints.at(name);
}

float TutorialData::GetFlt(const char* name) const
{
    return m_flts.at(name);
}

void TutorialData::SetBool(const char* name, bool value)
{
    m_ints.at(name) = value ? 1 : 0;
}

bool TutorialData::GetBool(const char* name) const
{
    return m_ints.at(name) != 0;
}

void TutorialData::CreateInt(const char* name, int value)
{
    m_ints[name] = value;
}

void TutorialData::CreateFlt(const char* name, float value)
{
    m_flts[name] = value;
}

void TutorialData::CreateBool(const char* name, bool value)
{
    m_ints[name] = value;
}

Input& TutorialData::Input() const
{
    return m_input;
}

PhysicsScene& TutorialData::Scene() const
{
    return m_scene;
}

TutorialTweaker& TutorialData::Tweaker() const
{
    return m_tweaker;
}

const glm::ivec2& TutorialData::Size() const
{
    return m_size;
}

void TutorialData::Reset()
{
    m_tweaker.Reset();
    m_scene.Reset();
    m_flts.clear();
    m_ints.clear();
}