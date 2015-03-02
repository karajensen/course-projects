////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>
#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>

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