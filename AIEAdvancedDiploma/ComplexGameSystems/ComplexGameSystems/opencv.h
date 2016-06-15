////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "opencv.hpp"
#include "directxcommon.h"

class DirectxEngine;

/**
* Wrapper for the OpenCV library
*/
class OpenCV
{
public:

    /**
    * Constructor
    */
    OpenCV(DirectxEngine& directx, int width, int height);

    /**
    * Destructor
    */
    ~OpenCV();

    /**
    * Closes OpenCV
    */
    void Close();

    /**
    * Initialises openCV
    * @return whether initialisation succeeded
    */
    bool Initialize();

    /**
    * Updates openCV
    */
    bool Update(double deltatime);

    /**
    * @return the camera frame
    */
    ID3D11Texture2D* GetFrame();

private:

    /**
    * Prevent copying
    */
    OpenCV(const OpenCV&) = delete;
    OpenCV& operator=(const OpenCV&) = delete;

    DirectxEngine& m_directx;
    ID3D11Texture2D* m_texture;
    cv::VideoCapture m_video;
    cv::Mat m_frame_bgr;
    cv::Mat m_frame_rgba;
    int m_width = 0;
    int m_height = 0;
    double m_timer = 0.0;
};
