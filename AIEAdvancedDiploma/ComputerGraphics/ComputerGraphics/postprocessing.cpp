////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "tweaker.h"

PostProcessing::PostProcessing()
{
    SetPostMap(FINAL_MAP);

    m_blurStep = 0.003f;
    m_depthFar = 1000.0f;
    m_depthNear = 1.0f;
    m_dofStart = 0.99f;
    m_dofFade = 0.01f;
    m_fogColour.r = 0.0f;
    m_fogColour.g = 100.0f/255.0f;
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
    tweaker.AddFltEntry("Saturation", &m_saturation, 0.01f);
    tweaker.AddFltEntry("Contrast", &m_contrast, 0.01f);
    tweaker.AddFltEntry("Blur Step", &m_blurStep, 0.0001f, 5);
    tweaker.AddFltEntry("Depth Far", &m_depthFar, 10.0f, 1);
    tweaker.AddFltEntry("Depth Near", &m_depthNear, 1.0f);
    tweaker.AddFltEntry("DOF Start", &m_dofStart, 0.01f);
    tweaker.AddFltEntry("DOF Fade", &m_dofFade, 0.01f);
    tweaker.AddFltEntry("Bloom Start", &m_bloomStart, 0.01f);
    tweaker.AddFltEntry("Bloom Fade", &m_bloomFade, 0.01f);
    tweaker.AddFltEntry("Bloom Intensity", &m_bloomIntensity, 0.01f);
    tweaker.AddFltEntry("Fog Start", &m_fogStart, 0.01f);
    tweaker.AddFltEntry("Fog Fade", &m_fogFade, 0.01f);
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