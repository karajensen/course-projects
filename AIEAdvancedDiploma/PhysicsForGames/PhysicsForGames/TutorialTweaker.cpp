////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialTweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialTweaker.h"
#include "Tweaker.h"
#include "PhysicsObject.h"
#include "SquareBody.h"
#include "Plane.h"
#include "CircleBody.h"
#include "Utilities.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <algorithm>
#include <assert.h>

TutorialTweaker::TutorialTweaker(Tweaker& tweaker,
                                 std::function<void(const char*, int)> setInt,
                                 std::function<void(const char*, float)> setFlt,
                                 std::function<int(const char*)> getInt,
                                 std::function<float(const char*)> getFlt)
    : m_tweaker(tweaker)
    , m_setInt(setInt)
    , m_setFlt(setFlt)
    , m_getInt(getInt)
    , m_getFlt(getFlt)
{
}

void TutorialTweaker::SetGroup(const char* group)
{
    m_tweaker.SetGroup(group);
}

void TutorialTweaker::AddButton(std::string label, 
                                std::function<void(void)> callback)
{
    m_tweaker.AddButton(label, callback);
}

void TutorialTweaker::AddTweakableFlt(const char* name, 
                                      const char* label, 
                                      float step,
                                      int precision,
                                      std::function<void(void)> onSet)
{
    AddTweakableFlt(name, label, step, -FLT_MAX, FLT_MAX, precision, onSet);
}

void TutorialTweaker::AddTweakableFlt(const char* name, 
                                      const char* label, 
                                      float step,
                                      float min,
                                      float max,
                                      int precision,
                                      std::function<void(void)> onSet)
{
    auto setFlt = [this, name, onSet, min, max](float value)
    {
        m_setFlt(name, std::max(std::min(value, max), min));

        if (onSet)
        {
            onSet();
        }
    };

    m_tweaker.AddFltEntry(label,
        [this, name]() { return m_getFlt(name); },
        setFlt, step, precision);
}

void TutorialTweaker::AddTweakableInt(const char* name, 
                                      const char* label,
                                      std::function<void(void)> onSet)
{
    AddTweakableInt(name, label, INT_MIN, INT_MAX, onSet);
}

void TutorialTweaker::AddTweakableInt(const char* name, 
                                      const char* label,
                                      int min, 
                                      int max,
                                      std::function<void(void)> onSet)
{
    auto setInt = [this, name, onSet, min, max](int value)
    {
        m_setInt(name, std::max(std::min(value, max), min));

        if (onSet)
        {
            onSet();
        }
    };

    m_tweaker.AddIntEntry(label,
        [this, name]() { return m_getInt(name); },
        setInt, INT_MAX);
}

void TutorialTweaker::AddTweakbleSquare(SquareBody* obj, 
                                        const std::string& label,
                                        std::function<void(void)> onSet)
{
    AddTweakbleObject(obj, label, onSet);

    m_tweaker.AddFltEntry((label + "Size X").c_str(),
        [obj]() { return obj->GetSize().x; },
        [obj](float value) { obj->SetSize(value, obj->GetSize().y); },
        1.0f, 1);

    m_tweaker.AddFltEntry((label + "Size Y").c_str(),
        [obj]() { return obj->GetSize().y; },
        [obj](float value) { obj->SetSize(obj->GetSize().x, value); },
        1.0f, 1);
}

void TutorialTweaker::AddTweakbleObject(PhysicsObject* obj, 
                                        const std::string& label,
                                        std::function<void(void)> onSet)
{
    m_tweaker.AddColorEntry((label + "Colour").c_str(),
        [obj]() { return obj->GetColour(); },
        [obj](const glm::vec4 value) { obj->SetColor(value); });

    m_tweaker.AddFltEntry((label + "Position X").c_str(),
        [obj]() { return obj->GetPosition().x; },
        [obj](float value) { obj->SetPosition(value, obj->GetPosition().y); },
        1.0f, 1);

    m_tweaker.AddFltEntry((label + "Position Y").c_str(),
        [obj]() { return obj->GetPosition().y; },
        [obj](float value) { obj->SetPosition(obj->GetPosition().x, value); },
        1.0f, 1);
}

void TutorialTweaker::AddTweakblePlane(Plane* obj,
                                       const std::string& label,
                                       std::function<void(void)> onSet)
{
    m_tweaker.AddFltEntry((label + " Normal X").c_str(),
        [obj]() { return obj->GetNormal().x; },
        [obj](float value) { obj->SetNormalX(value); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Normal Y").c_str(),
        [obj]() { return obj->GetNormal().y; },
        [obj](float value) { obj->SetNormalY(value); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Distance").c_str(),
        [obj]() { return obj->GetDistance(); },
        [obj](float value) { obj->SetDistance(value); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Size").c_str(),
        [obj]() { return obj->GetSize(); },
        [obj](float value) { obj->SetSize(value); },
        0.1f, 3);
}