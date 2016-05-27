////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opencv.h"
#include <Windows.h>
#include "opencv.hpp"

OpenCV::OpenCV()
{
}

OpenCV::~OpenCV()
{
}

bool OpenCV::Initialize()
{   
    cv::VideoCapture cap;
    if (!cap.open(0))
    {
        MessageBox(0, "Failed to open web cam", "ERROR", MB_OK);
        return false;
    }
        
    return true;
}