////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "tweaker.h"

PostProcessing::PostProcessing()
{
    SetPostMap(FINAL_MAP);

    m_blurStep = 0.004f;
    m_depthFar = 1000.0f;
    m_depthNear = 1.0f;
    m_dofStart = 1.0f;
    m_dofFade = 0.01f;
    m_fogColour.r = 0.0f;
    m_fogColour.g = 69.0f/255.0f;
    m_fogColour.b = 1.0f;
    m_fogStart = 0.39f;
    m_fogFade = 0.61f;
    m_bloomIntensity = 0.5f;
    m_bloomStart = 0.95f;
    m_bloomFade = 0.35f;
    m_contrast = 0.0f;
    m_saturation = 1.0f;
    m_maximumColour.r = 1.0f;
    m_maximumColour.g = 1.0f;
    m_maximumColour.b = 1.0f;
    m_minimumColour.r = 0.0f;
    m_minimumColour.g = 0.0f;
    m_minimumColour.b = 0.0f;

    m_weights[0] = 1.0f;
    m_weights[1] = 0.9f;
    m_weights[2] = 0.55f;
    m_weights[3] = 0.18f;
    m_weights[4] = 0.1f;

    // Normalise the blurring weights
    const float overallWeight = m_weights[0] + 2.0f * 
        (m_weights[1] + m_weights[2] + m_weights[3] + m_weights[4]);

    m_weights[0] /= overallWeight;
    m_weights[1] /= overallWeight;
    m_weights[2] /= overallWeight;
    m_weights[3] /= overallWeight;
    m_weights[4] /= overallWeight;
}

void PostProcessing::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddEntry("Saturation", &m_saturation, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Contrast", &m_contrast, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Blur Step", &m_blurStep, TW_TYPE_FLOAT, 0.01f, 4);
    tweaker.AddEntry("Depth Far", &m_depthFar, TW_TYPE_FLOAT, 10.0f, 1);
    tweaker.AddEntry("Depth Near", &m_depthNear, TW_TYPE_FLOAT, 1.0f);
    tweaker.AddEntry("DOF Start", &m_dofStart, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("DOF Fade", &m_dofFade, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Bloom Start", &m_bloomStart, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Bloom Fade", &m_bloomFade, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Bloom Intensity", &m_bloomIntensity, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Fog Start", &m_fogStart, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Fog Fade", &m_fogFade, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Fog Colour", &m_fogColour.x, TW_TYPE_COLOR3F);
    tweaker.AddEntry("Min Colour", &m_minimumColour.x, TW_TYPE_COLOR3F);
    tweaker.AddEntry("Max Colour", &m_maximumColour.x, TW_TYPE_COLOR3F);
}

float PostProcessing::BlurWeight(int index) const
{
    return m_weights.at(index);
}

std::string PostProcessing::GetPostMap() const
{
    return GetMapName(m_selectedMap);
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
    m_selectedMap = map;
    m_masks.assign(0.0f);
    m_masks[map] = 1.0f;
}

float PostProcessing::DOFStart() const
{
    return m_dofStart;
}

float PostProcessing::DOFFade() const
{
    return m_dofFade;
}

float PostProcessing::BloomIntensity() const
{
    return m_bloomIntensity;
}

float PostProcessing::BloomStart() const
{
    return m_bloomStart;
}

float PostProcessing::BloomFade() const
{
    return m_bloomFade;
}

float PostProcessing::Contrast() const
{
    return m_contrast;
}

float PostProcessing::Saturation() const
{
    return m_saturation;
}

float PostProcessing::BlurStep() const
{
    return m_blurStep;
}

float PostProcessing::DepthNear() const
{
    return m_depthNear;
}

float PostProcessing::DepthFar() const
{
    return m_depthFar;
}

float PostProcessing::FogStart() const
{
    return m_fogStart;
}

float PostProcessing::FogFade() const
{
    return m_fogFade;
}

const glm::vec3& PostProcessing::FogColour() const
{
    return m_fogColour;
}

const glm::vec3& PostProcessing::MinColour() const
{
    return m_minimumColour;
}

const glm::vec3& PostProcessing::MaxColour() const
{
    return m_maximumColour;
}

float PostProcessing::Mask(PostProcessing::Map map) const
{
    return m_masks.at(map);
}

std::string PostProcessing::GetMapName(PostProcessing::Map map)
{
    switch (map)
    {
    case FINAL_MAP:
        return "Final Scene";
    case SCENE_MAP:
        return "Scene Map";
    case NORMAL_MAP:
        return "Normal Map";
    case DEPTH_MAP:
        return "Depth Map";
    case BLUR_MAP:
        return "Blur Map";
    case DOF_MAP:
        return "DOF Map";
    case FOG_MAP:
        return "Fog Map";
    case BLOOM_MAP:
        return "Bloom Map";
    default:
        return "None";
    }
}