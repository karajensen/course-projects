////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

const int NO_INDEX = -1;

/**
* Converts degrees to radians
*/
template<typename T> T DegToRad(T degrees)
{
    return static_cast<T>(M_PI/180.0)*degrees;
}

/**
* Converts radians to degrees
*/
template<typename T> T RadToDeg(T radians)
{
    return static_cast<T>(180.0/M_PI)*radians;
}

/**
* Logs info to the outputstream
* @param info The information to log
*/
inline void LogInfo(const std::string& info)
{
    #ifdef _DEBUG
        std::cout << "INFO: \t" << info << std::endl;
    #endif
}

/**
* Logs error to the outputstream
* @param error The error to log
*/
inline void LogError(const std::string& error)
{
    #ifdef _DEBUG
        std::cout << "ERROR: \t" << error << std::endl;
    #endif
}

/**
* Case insensitive comparison between two strings
*/
inline bool IsStrEqual(const std::string& str1, const std::string& str2)
{
    return _stricmp(str1.c_str(), str2.c_str()) == 0;
}

/**
* Change the range a value is between
*/
template<typename T> T ConvertRange(T value,    
                                    T currentInner, 
                                    T currentOuter, 
                                    T newInner, 
                                    T newOuter)
{
    return ((value-currentInner)*((newOuter-newInner)/
        (currentOuter-currentInner)))+newInner;
}

/**
* Clamp a value between min and max
*/
template<typename T> T Clamp(T value, T minValue, T maxValue)
{
    return std::min(std::max(value, minValue), maxValue);
}

/**
* Utility class to get a random value
*/
struct Random
{
    static int Generate(int min, int max)
    {
        static std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    static float Generate(float min, float max)
    {
        static std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
};
