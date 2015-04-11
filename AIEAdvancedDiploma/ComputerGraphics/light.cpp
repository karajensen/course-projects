////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "light.h"
#include "tweaker.h"

Light::Light(const std::string& name) :
    m_name(name)
{
}

void Light::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddEntry("Name", [this](){ return m_name; });
    tweaker.AddEntry("Position X", &m_position.x, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Position Y", &m_position.y, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Position Z", &m_position.z, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Attenuation X", &m_attenuation.x, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Attenuation Y", &m_attenuation.y, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Attenuation Z", &m_attenuation.z, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Specularity", &m_specularity, TW_TYPE_FLOAT, 0.01f);
    tweaker.AddEntry("Specular", &m_specular.x, TW_TYPE_COLOR3F);
    tweaker.AddEntry("Diffuse", &m_diffuse.x, TW_TYPE_COLOR3F);
}

const std::string& Light::Name() const
{
    return m_name;
}

const glm::vec3& Light::Diffuse() const
{
    return m_diffuse;
}

const glm::vec3& Light::Specular() const
{
    return m_specular;
}

const glm::vec3& Light::Attenuation() const
{
    return m_attenuation;
}

const glm::vec3& Light::Position() const
{
    return m_position;
}

float Light::Specularity() const
{
    return m_specularity;
}

float Light::Active() const
{
    return m_active;
}

void Light::Diffuse(const glm::vec3& value)
{
    m_diffuse = value;
}

void Light::Specular(const glm::vec3& value)
{
    m_specular = value;
}

void Light::Attenuation(const glm::vec3& value)
{
    m_attenuation = value;
}

void Light::Position(const glm::vec3& value)
{
    m_position = value;
}

void Light::Specularity(float value)
{
    m_specularity = value;
}

void Light::Active(float value)
{
    m_active = value;
}

