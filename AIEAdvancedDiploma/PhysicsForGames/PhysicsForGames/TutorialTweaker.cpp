////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialTweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialTweaker.h"
#include "TutorialData.h"
#include "Tweaker.h"
#include "PhysicsObject.h"
#include "SquareBody.h"
#include "Plane.h"
#include "CircleBody.h"
#include "Utilities.h"
#include "SpringJoint.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <algorithm>
#include <assert.h>

TutorialTweaker::TutorialTweaker(Tweaker& tweaker)
    : m_tweaker(tweaker)
{
}

void TutorialTweaker::Reset()
{
    m_tweaker.Reset();
}

void TutorialTweaker::SetData(TutorialData* data)
{
    m_data = data;
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
    assert(m_data);

    auto setFlt = [this, name, onSet, min, max](float value)
    {
        m_data->SetFlt(name, std::max(std::min(value, max), min));

        if (onSet)
        {
            onSet();
        }
    };

    m_tweaker.AddFltEntry(label,
        [this, name]() { return m_data->GetFlt(name); },
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
    assert(m_data);

    auto setInt = [this, name, onSet, min, max](int value)
    {
        m_data->SetInt(name, std::max(std::min(value, max), min));

        if (onSet)
        {
            onSet();
        }
    };

    m_tweaker.AddIntEntry(label,
        [this, name]() { return m_data->GetInt(name); },
        setInt, INT_MAX);
}

void TutorialTweaker::AddTweakableBool(const char* name,
                                       const char* label,
                                       std::function<void(void)> onSet)
{
    assert(m_data);

    auto setter = [this, name, onSet](bool value)
    {
        m_data->SetBool(name, value);

        if (onSet)
        {
            onSet();
        }
    };

    m_tweaker.AddBoolEntry(label,
        [this, name]() { return m_data->GetBool(name); },
        setter);
}

void TutorialTweaker::AddTweakbleSquare(SquareBody* obj, 
                                        const std::string& label,
                                        std::function<void(void)> onSet)
{
    if (!onSet)
    {
        onSet = [](){};
    }

    m_tweaker.AddFltEntry((label + "Size X").c_str(),
        [obj]() { return obj->GetSize().x; },
        [obj, onSet](float value) { obj->SetSize(value, obj->GetSize().y); onSet(); },
        1.0f, 1);

    m_tweaker.AddFltEntry((label + "Size Y").c_str(),
        [obj]() { return obj->GetSize().y; },
        [obj, onSet](float value) { obj->SetSize(obj->GetSize().x, value); onSet(); },
        1.0f, 1);

    m_tweaker.AddFltEntry((label + "Position X").c_str(),
        [obj]() { return obj->GetPosition().x; },
        [obj, onSet](float value) { obj->SetPosition(value, obj->GetPosition().y); onSet(); },
        1.0f, 1);

    m_tweaker.AddFltEntry((label + "Position Y").c_str(),
        [obj]() { return obj->GetPosition().y; },
        [obj, onSet](float value) { obj->SetPosition(obj->GetPosition().x, value); onSet(); },
        1.0f, 1);
}

void TutorialTweaker::AddTweakbleObject(PhysicsObject* obj, 
                                        const std::string& label,
                                        std::function<void(void)> onSet)
{
    if (!onSet)
    {
        onSet = []() {};
    }

    m_tweaker.AddColorEntry((label + "Colour").c_str(),
        [obj]() { return obj->GetColour(); },
        [obj, onSet](const glm::vec4 value) { obj->SetColor(value); onSet(); });
}

void TutorialTweaker::AddTweakbleJoint(SpringJoint* obj,
                                       const std::string& label,
                                       std::function<void(void)> onSet)
{
    if (!onSet)
    {
        onSet = []() {};
    }

    m_tweaker.AddFltEntry((label + " Spring Coefficient").c_str(),
        [obj]() { return obj->GetSpringCoefficient(); },
        [obj, onSet](float value) { obj->SetSpringCoefficient(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Damping").c_str(),
        [obj]() { return obj->GetDamping(); },
        [obj, onSet](float value) { obj->SetDamping(value); onSet(); },
        0.1f, 3);
}

void TutorialTweaker::AddTweakblePlane(Plane* obj,
                                       const std::string& label,
                                       std::function<void(void)> onSet)
{
    if (!onSet)
    {
        onSet = []() {};
    }

    m_tweaker.AddFltEntry((label + " Normal X").c_str(),
        [obj]() { return obj->GetNormal().x; },
        [obj, onSet](float value) { obj->SetNormalX(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Normal Y").c_str(),
        [obj]() { return obj->GetNormal().y; },
        [obj, onSet](float value) { obj->SetNormalY(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Distance").c_str(),
        [obj]() { return obj->GetDistance(); },
        [obj, onSet](float value) { obj->SetDistance(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Size").c_str(),
        [obj]() { return obj->GetSize(); },
        [obj, onSet](float value) { obj->SetSize(value); onSet(); },
        0.1f, 3);
}

void TutorialTweaker::AddTweakbleBody(RigidBody* obj,
                                      const std::string& label,
                                      std::function<void(void)> onSet)
{
    if (!onSet)
    {
        onSet = []() {};
    }

    m_tweaker.AddFltEntry((label + " Mass").c_str(),
        [obj]() { return obj->GetMass(); },
        [obj, onSet](float value) { obj->SetMass(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Radius").c_str(),
        [obj]() { return obj->GetRadius(); },
        [obj, onSet](float value) { obj->SetRadius(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Gravity").c_str(),
        [obj]() { return obj->GetGravity().y; },
        [obj, onSet](float value) { obj->SetGravity(obj->GetGravity().x, value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Linear Drag").c_str(),
        [obj]() { return obj->GetLinearDrag(); },
        [obj, onSet](float value) { obj->SetLinearDrag(value); onSet(); },
        0.1f, 3);

    m_tweaker.AddFltEntry((label + " Angular Drag").c_str(),
        [obj]() { return obj->GetAngularDrag(); },
        [obj, onSet](float value) { obj->SetAngularDrag(value); onSet(); },
        0.1f, 3);
}