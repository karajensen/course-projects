////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderdata.h"
#include <array>

class Tweaker;

/**
* Holds data for post processing the final image
*/
class PostProcessing
{
public:

    /**
    * Post Shader maps
    */
    enum Map
    {
        FINAL_MAP,
        SCENE_MAP,
        DEPTH_MAP,
        BLUR_MAP,
        DOF_MAP,
        FOG_MAP,
        BLOOM_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(Map map);

    /**
    * @return Distance the depth of field starts
    */
    float DOFStart() const;

    /**
    * @return How quick depth of field fades to the scene
    */
    float DOFFade() const;

    /**
    * @param index The pixel to get the weight for
    * @return The blur weight for the surrounding pixels
    */
    float BlurWeight(int index) const;

    /**
    * @return Contrast controller of the final scene
    */
    float Contrast() const;

    /**
    * @return Saturation controller of the final scene
    */
    float Saturation() const;

    /**
    * @return Sampling step for blurring
    */
    float BlurStep() const;

    /**
    * @return Value where depth colour is min
    */
    float DepthNear() const;

    /**
    * @return Value where depth colour is max
    */
    float DepthFar() const;

    /**
    * @return Distance the fog starts
    */
    float FogStart() const;

    /**
    * @return How quick fog fades to the scene
    */
    float FogFade() const;

    /**
    * @return Colour for the fog
    */
    const glm::vec3& FogColour() const;

    /**
    * @return Minimum Colour ranges for RGB
    */
    const glm::vec3& MinColour() const;

    /**
    * @return Maximum Colour ranges for RGB
    */
    const glm::vec3& MaxColour() const;

    /**
    * @return The mask value for the map
    */
    float Mask(PostProcessing::Map map) const;

    /**
    * @return The itensity of the bloom
    */
    float BloomIntensity() const;

    /**
    * @return The threshold when the bloom starts
    */
    float BloomStart() const;

    /**
    * @return The fadeoff of the bloom
    */
    float BloomFade() const;

    /**
    * @return the currently rendered map
    */
    std::string GetPostMap() const;

    /**
    * @param map the map type to convert
    * @return The string name of the map type
    */
    static std::string GetMapName(Map map);

private:

    /**
    * Prevent copying
    */
    PostProcessing(const PostProcessing&) = delete;
    PostProcessing& operator=(const PostProcessing&) = delete;

    float m_dofStart = 0.0f;              ///< Distance the depth of field starts
    float m_dofFade = 0.0f;               ///< How quick depth of field fades to the scene
    float m_bloomStart = 0.0f;            ///< The threshold when the bloom starts
    float m_bloomIntensity = 0.0f;        ///< Intensity of the bloom
    float m_bloomFade = 0.0f;             ///< The amount of fade off
    float m_contrast = 0.0f;              ///< Contrast controller of the final scene
    float m_saturation = 0.0f;            ///< Saturation controller of the final scene
    float m_blurStep = 0.0f;              ///< Sampling step for blurring
    float m_depthNear = 0.0f;             ///< Value where depth colour is min
    float m_depthFar = 0.0f;              ///< Value where depth colour is max
    float m_fogStart = 0.0f;              ///< Distance the fog starts
    float m_fogFade = 0.0f;               ///< How quick fog fades to the scene
    glm::vec3 m_fogColour;                ///< Colour for the fog
    glm::vec3 m_minimumColour;            ///< Minimum Colour ranges for RGB
    glm::vec3 m_maximumColour;            ///< Maximum Colour ranges for RGB
    std::array<float, MAX_MAPS> m_masks;  ///< Visibility of post maps
    Map m_selectedMap;                    ///< The currently rendered post map

    static const int BLUR_PIXELS = 5;          ///< Amount of pixels per-pixel involved in blurring
    std::array<float, BLUR_PIXELS> m_weights;  ///< Normalised pixel weights for blurring
};