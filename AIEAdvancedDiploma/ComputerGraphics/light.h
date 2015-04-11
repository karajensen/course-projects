////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "glm\glm.hpp"

class Tweaker;

/**
* Single light to provide illumination in the scene
*/
class Light
{
public:

    /**
    * Index for avaliable lights
    */
    enum Index
    {
        ID_SUN,
        ID_SPOT,
        MAX_LIGHTS
    };

    /**
    * Constructor
    * @param name The name of the light
    */
    Light(const std::string& name); 

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * @return The name of the light
    */
    const std::string& Name() const;

    /**
    * @return Colour of the light
    */
    const glm::vec3& Diffuse() const;

    /**
    * @return Specular highlights the light will cast
    */
    const glm::vec3& Specular() const;

    /**
    * @return How much the light will fade in distance
    */
    const glm::vec3& Attenuation() const;

    /**
    * @return World coordinates of the light
    */
    const glm::vec3& Position() const;

    /**
    * @return The brightness of the specular highlights
    */
    float Specularity() const;

    /**
    * @return How much activity this light contributes
    */
    float Active() const;

    /**
    * Sets the Colour of the light
    */
    void Diffuse(const glm::vec3& value);

    /**
    * Sets the Specular highlights the light will cast
    */
    void Specular(const glm::vec3& value);

    /**
    * Sets How much the light will fade in distance
    */
    void Attenuation(const glm::vec3& value);

    /**
    * Sets the World coordinates of the light
    */
    void Position(const glm::vec3& value);

    /**
    * Sets The brightness of the specular highlights
    */
    void Specularity(float value);

    /**
    * Sets How much activity this light contributes
    */
    void Active(float value);

private:

    /**
    * Prevent copying
    */
    Light(const Light&) = delete;
    Light& operator=(const Light&) = delete;
                                 
    const std::string m_name;     ///< Name of the light
    glm::vec3 m_diffuse;          ///< Colour of the light
    glm::vec3 m_specular;         ///< Specular highlights the light will cast
    glm::vec3 m_attenuation;      ///< How much the light will fade in distance
    glm::vec3 m_position;         ///< World coordinates of the light
	float m_specularity = 1.0f;   ///< Brightness of the specular highlights
    float m_active = 1.0f;        ///< How much activity this light contributes
};                                  