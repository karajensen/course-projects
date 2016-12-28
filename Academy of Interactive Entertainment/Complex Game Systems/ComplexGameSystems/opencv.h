////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "opencv.hpp"
#include "directxcommon.h"

class Tweaker;

/**
* Wrapper for the OpenCV library
*/
class OpenCV
{
public:

    /**
    * Constructor
    */
    OpenCV(ID3D11Device* device,
           ID3D11DeviceContext* context,
           const POINT& size);

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
    * @return whether theres a new frame avaliable
    */
    bool Update();

    /**
    * @return the camera frame
    */
    ID3D11Texture2D* GetFrame();

    /**
    * Adds to the tweak bar
    */
    void AddToTweaker(Tweaker& tweaker);

private:

    /**
    * Prevent copying
    */
    OpenCV(const OpenCV&) = delete;
    OpenCV& operator=(const OpenCV&) = delete;

    ID3D11DeviceContext* m_context = nullptr;
    ID3D11Device* m_device = nullptr;
    ID3D11Texture2D* m_texture;
    cv::VideoCapture m_video;
    cv::Mat m_frame_bgr;
    cv::Mat m_frame_rgba;
    int m_width = 0;
    int m_height = 0;
};
