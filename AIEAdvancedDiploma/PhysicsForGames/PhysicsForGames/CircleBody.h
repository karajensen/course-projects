////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sphere.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"

class CircleBody : public RigidBody
{
public:

    /**
    * Constructor
    * @param position The position to initialise the body at
    * @param velocity The initial velocity of the body
    * @param mass The mass of the body
    * @param radius The size of the circle
    * @param colour What colour to render the body with
    */
    CircleBody(const glm::vec2& position, 
               const glm::vec2& velocity, 
               float mass, 
               float radius, 
               const glm::vec4& colour);

    /**
    * Updates the body
    * @param timeStep The time between each update call
    */
    virtual void Update(float timeStep) override;

    /**
    * Renders the body
    * @param renderer The 2D renderer for drawing the body
    */
    virtual void Draw(aie::Renderer2D* renderer) override;

    /**
    * Outputs debug information about the body
    */
    virtual void Debug() override;

    /**
    * Sets the size of the circle
    */
    void SetRadius(float radius);

    /**
    * @return the size of the circle
    */
    float GetRadius() const;

private:

    /**
    * Prevent Copying
    */
    CircleBody(const CircleBody&) = delete;
    CircleBody& operator=(const CircleBody&) = delete;

    float m_radius = 0.0f;  ///< The size of the circle
};