////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SpringJoint.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SpringJoint.h"
#include "Renderer2D.h"
#include "RigidBody.h"
#include "glm/glm.hpp"

SpringJoint::SpringJoint(RigidBody& connectionA,
                         RigidBody& connectionB, 
                         float springCoefficient,
                         float damping)
    : PhysicsObject(NO_SHAPE, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
    , m_connectionA(connectionA)
    , m_connectionB(connectionB)
    , m_springCoefficient(springCoefficient)
    , m_damping(damping)
{
    m_restLength = glm::length(connectionA.GetPosition() - connectionB.GetPosition());
    
    SetCollidable(false);
}

void SpringJoint::Update(float timeStep)
{
    // Update: This is where we implement Hook’s law. Refer to the slides to see how to do this.
    // Don’t worry about damping the spring for the time being. (hint: start by creating an undamped
    // spring. This is a matter of working out the restitution force from the difference
    // between the rest distance and the current distance between the balls, multiplying by the
    // spring coefficient and applying the force to the balls (note that one is fixed).




}

void SpringJoint::Draw(aie::Renderer2D& renderer, bool showDiagnostics)
{
    PhysicsObject::Draw(renderer, showDiagnostics);

    renderer.drawLine(m_connectionA.GetPosition().x,
                      m_connectionA.GetPosition().y,
                      m_connectionB.GetPosition().x,
                      m_connectionB.GetPosition().y);
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
    m_damping = value;
}

float SpringJoint::GetDamping() const
{
    return m_damping;
}