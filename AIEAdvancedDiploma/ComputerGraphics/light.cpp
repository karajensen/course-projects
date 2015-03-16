////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "light.h"

Light::Light(const std::string& name) :
    m_name(name)
{
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

