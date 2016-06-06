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
    void Update(int vectorization);

private:

    /**
    * Prevent copying
    */
    OpenCV(const OpenCV&) = delete;
    OpenCV& operator=(const OpenCV&) = delete;

    /**
    * Adds diagnostic text
    */
    void DiagnosticLine(cv::UMat& mat, const std::string& text);

    DirectxEngine& m_directx;
    ID3D11Texture2D* m_texture;
    cv::VideoCapture m_video;
    cv::Mat m_frame_bgr;
    cv::Mat m_frame_rgba;
    cv::Point m_diagnosticPosition;
    int m_diagnosticLine = 0;
    int m_width = 0;
    int m_height = 0;
};
