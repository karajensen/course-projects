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
    * @param springCoefficient The stiffness of the spring
    * @param damping How quickly the spring will come to rest
    * @param color The colour of the joint
    */
    SpringJoint(RigidBody* connectionA, 
                RigidBody* connectionB, 
                float springCoefficient,
                float damping, 
                glm::vec4 color);

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
    * Sets the stiffness of the spring
    */
    void SetSpringCoefficient(float value);

    /**
    * Gets the stiffness of the spring
    */
    float GetSpringCoefficient() const;

    /**
    * Sets how quickly the spring will come to rest
    */
    void SetDamping(float value);

    /**
    * Gets how quickly the spring will come to rest
    */
    float GetDamping() const;

    /**
    * Resets the joint
    */
    void Reset();

private:

    /**
    * Prevent Copying
    */
    SpringJoint(const SpringJoint&) = delete;
    SpringJoint& operator=(const SpringJoint&) = delete;

    RigidBody* m_connectionA = nullptr; ///< First connecting body
    RigidBody* m_connectionB = nullptr; ///< Second connecting body
    float m_springCoefficient = 0.0f;   ///< The stiffness of the spring
    float m_damping = 1.0f;             ///< How quickly the spring will come to rest
    float m_restLength = 0.0f;          ///< Initial length between the two connecting bodies
};