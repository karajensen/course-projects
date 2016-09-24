////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Utilities.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES

#include "glm/vec2.hpp"
#include <iostream>
#include <math.h>

/**
* Convert degrees to radians
*/
template<typename T> T DegToRad(T degrees)
{
    return static_cast<T>(M_PI / 180.0) * degrees;
}

/**
* Convert radians to degrees
*/
template<typename T> T RadToDeg(T radians)
{
    return static_cast<T>(180.0 / M_PI) * radians;
}

/**
* Logs an error to the console
*/
template<typename T> void LogError(const T& message)
{
    std::cout << "ERROR: " << message << "\n";
}

/**
* Logs a message to the console
*/
template<typename T> void LogInfo(const T& message)
{
    std::cout << "INFO: " << message << "\n";
}

class Utils
{
public:

    /**
    * Sets a vector to zero
    */
    static void SetZero(glm::vec2& vec)
    {
        vec.x = 0.0f;
        vec.y = 0.0f;
    }

    /**
    * @return whether a vector is set to zero
    */
    static bool IsZero(glm::vec2& vec)
    {
        return vec.x == 0.0f && vec.y == 0.0f;
    }
};  