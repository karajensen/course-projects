////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include <map>

class PhysicsScene;
class TutorialTweaker;
class Input;

/**
* Data for creating and managing the currently selected tutorial scene
*/
class TutorialData
{
public:

    /**
    * Constructor
    * @param input For querying user input
    * @param scene the physics scene manager
    * @param tweaker The tweak bar
    * @param size The size of the window
    */
    TutorialData(Input& input, 
                 PhysicsScene& scene, 
                 TutorialTweaker& tweaker,
                 const glm::ivec2& size);

    /**
    * Sets tutorial int data
    */
    void SetInt(const char* name, int value);

    /**
    * Sets tutorial float data
    */
    void SetFlt(const char* name, float value);

    /**
    * Gets tutorial int data
    */
    int GetInt(const char* name) const;

    /**
    * Gets tutorial float data
    */
    float GetFlt(const char* name) const;

    /**
    * Sets the tutorial bool data
    */
    void SetBool(const char* name, bool value);

    /**
    * Gets the tutorial bool data
    */
    bool GetBool(const char* name) const;

    /**
    * Creates a new int value
    */
    void CreateInt(const char* name, int value);

    /**
    * Creates a new float value
    */
    void CreateFlt(const char* name, float value);

    /**
    * Creates a bool value
    */
    void CreateBool(const char* name, bool value);

    /**
    * @return the input manager for querying user input
    */
    Input& Input() const;

    /**
    * @return the physics scene manager
    */
    PhysicsScene& Scene() const;

    /**
    * @return the tutorial tweaker
    */
    TutorialTweaker& Tweaker() const;

    /**
    * @return the size of the window
    */
    const glm::ivec2& Size() const;

    /**
    * Resets any stored tutorial data
    */
    void Reset();

private:
    /**
    * Prevent Copying
    */
    TutorialData(const TutorialData&) = delete;
    TutorialData& operator=(const TutorialData&) = delete;

    ::Input& m_input;                      ///< For querying user input
    PhysicsScene& m_scene;                 ///< Physics scene manager
    TutorialTweaker& m_tweaker;            ///< For tweaking the tutorial
    const glm::ivec2& m_size;              ///< Size of the window
    std::map<const char*, float> m_flts;   ///< Current tutorial specific parameters
    std::map<const char*, int> m_ints;     ///< Current tutorial specific parameters
};