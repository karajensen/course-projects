////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"

PostProcessing::PostProcessing()
{
    SetPostMap(SCENE_MAP);

    m_blurStep = 0.004f;
    m_depthFar = 1000.0f;
    m_depthNear = 1.0f;
    m_dofStart = 1.0f;
    m_dofFade = 0.01f;
    m_fogColour.r = 0.0f;
    m_fogColour.g = 0.0f;
    m_fogColour.b = 1.0f;
    m_fogStart = -1.46f;
    m_fogFade = 2.4f;
    m_bloomIntensity = 1.0f;
    m_bloomStart = 0.9f;
    m_bloomFade = 0.1f;
    m_contrast = 0.0f;
    m_saturation = 1.0f;
    m_maximumColour.r = 1.0f;
    m_maximumColour.g = 1.0f;
    m_maximumColour.b = 1.0f;
    m_minimumColour.r = 0.0f;
    m_minimumColour.g = 0.0f;
    m_minimumColour.b = 0.0f;
    m_weightMain = 1.0f;
    m_weightOffset.x = 0.9f;
    m_weightOffset.y = 0.55f;
    m_weightOffset.z = 0.18f;
    m_weightOffset.w = 0.1f;

    // Normalise the blurring weights
    const float overallWeight = m_weightMain + 2.0f * 
        (m_weightOffset.x + m_weightOffset.y + m_weightOffset.z + m_weightOffset.w);

    m_weightMain /= overallWeight;
    m_weightOffset.x /= overallWeight;
    m_weightOffset.y /= overallWeight;
    m_weightOffset.z /= overallWeight;
    m_weightOffset.w /= overallWeight;
}

float PostProcessing::BlurWeightMain() const
{
    return m_weightMain;
}

const glm::vec4& PostProcessing::BlurWeightOffset() const
{
    return m_weightOffset;
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
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
    case AMBIENCE_MAP:
        return "Ambience Map";
    default:
        return "None";
    }
}