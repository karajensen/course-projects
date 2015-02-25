////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>
#include "opengl/gl_core_4_4.h"
#include "glfw/glfw3.h"

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