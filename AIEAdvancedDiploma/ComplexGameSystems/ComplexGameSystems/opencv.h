////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opencv.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Wrapper for the OpenCV library
*/
class OpenCV
{
public:

    /**
    * Constructor
    */
    OpenCV();

    /**
    * Destructor
    */
    ~OpenCV();

    /**
    * Initialises openCV
    * @return whether initialisation succeeded
    */
    bool Initialize();

private:

    /**
    * Prevent copying
    */
    OpenCV(const OpenCV&) = delete;
    OpenCV& operator=(const OpenCV&) = delete;
};
