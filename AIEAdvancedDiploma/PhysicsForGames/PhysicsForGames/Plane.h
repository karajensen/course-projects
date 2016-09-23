////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Plane.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:

    /**
    * Constructor
    * @param colour What colour to render the body with
    */
    Plane(const glm::vec4& colour);

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

private:

    /**
    * Prevent Copying
    */
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;

    float m_size = 0.0f;    ///< Dimentions of the plane
};