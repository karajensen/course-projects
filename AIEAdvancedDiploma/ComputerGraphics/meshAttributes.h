////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshAttributes.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Attributes used for shading meshes
*/
class MeshAttributes
{
public:

    /**
    * @return Brightness of the specular highlights
    */
    float Specularity() const;

    /**
    * Sets the Brightness of the specular highlights
    */
    void Specularity(float value);

    /**
    * @return The saturation of the bump
    */
    float Bump() const;

    /**
    * Sets The saturation of the bump
    */
    void Bump(float value);

    /**
    * @return Ambient light multiplier
    */
    float Ambience() const;

    /**
    * Sets the Ambient light multiplier
    */
    void Ambience(float value);

    /**
    * @return Diffuse light multiplier
    */
    float Diffuse() const;

    /**
    * Sets the minimum Diffuse light value
    */
    void Diffuse(float value);

    /**
    * @return Overall Specular light multiplier
    */
    float Specular() const;

    /**
    * Sets the Specular light multiplier
    */
    void Specular(float value);

    /**
    * @return Caustics multiplier
    */
    float CausticsAmount() const;

    /**
    * Sets the Caustics multiplier
    */
    void CausticsAmount(float value);

    /**
    * @return Caustics scale
    */
    float CausticsScale() const;

    /**
    * Sets the Caustics scale
    */
    void CausticsScale(float value);

protected:

    float m_bump = 1.0f;           ///< Saturation of bump
    float m_causticsAmount = 1.0f; ///< How much of the caustics are visible
    float m_causticsScale = 1.0f;  ///< The scale of the caustic texture
    float m_specularity = 1.0f;    ///< Brightness of the specular highlights
    float m_ambience = 1.0f;       ///< Ambient light multiplier
    float m_specular = 1.0f;       ///< Overall Specular light multiplier
    float m_diffuse = 0.0f;        ///< Minimum Diffuse light value
};