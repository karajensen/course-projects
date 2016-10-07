////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <map>

class PhysicsScene;
class TutorialTweaker;
class Input;

struct TutorialData
{
    Input* input = nullptr;              ///< For querying user input
    PhysicsScene* scene = nullptr;       ///< Physics scene manager
    TutorialTweaker* tweaker = nullptr;  ///< For tweaking the tutorial
    glm::ivec2 size;                     ///< Size of the window
    std::map<const char*, float> flts;   ///< Current tutorial specific parameters
    std::map<const char*, int> ints;     ///< Current tutorial specific parameters
};