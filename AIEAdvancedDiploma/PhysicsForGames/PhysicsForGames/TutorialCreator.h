////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialCreator.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <map>
#include <functional>

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
    TUTORIAL_COUNT
};

class TutorialCreator
{
public:
    TutorialCreator(aie::Input& input, PhysicsScene& scene, const glm::ivec2& size);
    void Create(Tweaker& tweaker, int tutorial);
    const char* GetName(int tutorial) const;
    Tutorial Selected() const;
    void AddToTweaker(Tweaker& tweaker);

private:
    void CreateTutorial1(Tweaker& tweaker);

    aie::Input& m_input;
    PhysicsScene& m_scene;
    const glm::ivec2& m_size;
    Tutorial m_currentTutorial = TUTORIAL_NONE;
    std::map<const char*, float> m_flts;
    std::map<const char*, int> m_ints;
    std::function<void(void)> m_resetTweaker = nullptr;
};