////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SpringJoint.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SpringJoint.h"
#include "Renderer2D.h"
#include "RigidBody.h"
#include "glm/glm.hpp"

SpringJoint::SpringJoint(RigidBody* connectionA,
                         RigidBody* connectionB, 
                         float springCoefficient,
                         float damping,
                         glm::vec4 color)
    : PhysicsObject(NO_SHAPE, color)
    , m_connectionA(connectionA)
    , m_connectionB(connectionB)
    , m_springCoefficient(springCoefficient)
    , m_damping(damping)
{
    Reset();
    SetCollidable(false);
}

void SpringJoint::Reset()
{
    m_restLength = glm::length(m_connectionA->GetPosition() - m_connectionB->GetPosition());
}

void SpringJoint::Update(float timeStep)
{
    if (m_connectionA->IsVisible() && m_connectionB->IsVisible())
    {
        // Hooke's Law F = -kX - bv
        const auto relativeVelocity = m_connectionA->GetVelocity() - m_connectionB->GetVelocity();
        const auto bodyDifference = m_connectionA->GetPosition() - m_connectionB->GetPosition();
        const auto restDifference = bodyDifference - glm::normalize(bodyDifference) * m_restLength;
        const auto force = (restDifference * -m_springCoefficient) - (relativeVelocity * (1.0f - m_damping));

        if (m_connectionA->IsActive() && m_connectionA->CanMoveDynamically())
        {
            m_connectionA->ApplyForce(force * 0.5f);
        }

        if (m_connectionB->IsActive() && m_connectionB->CanMoveDynamically())
        {
            m_connectionB->ApplyForce(-force * 0.5f);
        }
    }
}

void SpringJoint::Draw(aie::Renderer2D& renderer, bool showDiagnostics)
{
    if (m_connectionA->IsVisible() && m_connectionB->IsVisible())
    {
        PhysicsObject::Draw(renderer, showDiagnostics);

        renderer.drawLine(m_connectionA->GetPosition().x,
            m_connectionA->GetPosition().y,
            m_connectionB->GetPosition().x,
            m_connectionB->GetPosition().y);
    }
}

void SpringJoint::SetSpringCoefficient(float value)
{
    m_springCoefficient = value;
}

float SpringJoint::GetSpringCoefficient() const
{
    return m_springCoefficient;
}

void SpringJoint::SetDamping(float value)
{
    m_damping = std::max(std::min(1.0f, value), 0.0f);
}

float SpringJoint::GetDamping() const
{
    return m_damping;
}