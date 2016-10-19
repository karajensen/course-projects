////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SpringJoint.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

class RigidBody;

/**
* Joint for connecting two bodies together
*/
class SpringJoint : public PhysicsObject
{
public:

    /**
    * Constructor
    * @param connectionA First connecting body
    * @param connectionB Second connecting body
    * @param springCoefficient
    * @param damping 
    */
    SpringJoint(RigidBody& connectionA, 
                RigidBody& connectionB, 
                float springCoefficient,
                float damping);

    /**
    * Updates the joint
    * @param timeStep The time between each update call
    */
    virtual void Update(float timeStep) override;

    /**
    * Renders the joint
    * @param renderer The 2D renderer for drawing the joint
    * @param showDiagnostics Whether to render diagnostics
    */
    virtual void Draw(aie::Renderer2D& renderer, bool showDiagnostics) override;

    /**
    * Sets the spring coefficient
    */
    void SetSpringCoefficient(float value);

    /**
    * Gets the spring coefficient
    */
    float GetSpringCoefficient() const;

    /**
    * Sets the damping
    */
    void SetDamping(float value);

    /**
    * Gets the damping
    */
    float GetDamping() const;

private:

    /**
    * Prevent Copying
    */
    SpringJoint(const SpringJoint&) = delete;
    SpringJoint& operator=(const SpringJoint&) = delete;

    RigidBody& m_connectionA;           ///< First connecting body
    RigidBody& m_connectionB;           ///< Second connecting body
    float m_springCoefficient = 0.0f;   ///< 
    float m_damping = 0.0f;             ///< 
    float m_restLength = 0.0f;          ///< Initial length between the two connecting bodies
};