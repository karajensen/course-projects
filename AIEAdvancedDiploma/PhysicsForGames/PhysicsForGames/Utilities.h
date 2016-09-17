////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Utilities.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <iostream>

template<typename T> void LogError(const T& message)
{
    std::cout << "ERROR: " << message << "\n";
}

template<typename T> void LogInfo(const T& message)
{
    std::cout << "INFO: " << message << "\n";
}

class Utils
{
public:

    static void SetZero(glm::vec2& vec)
    {
        vec.x = 0.0f;
        vec.y = 0.0f;
    }

    static bool IsZero(glm::vec2& vec)
    {
        return vec.x == 0.0f && vec.y == 0.0f;
    }
};  