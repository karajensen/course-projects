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
    * @param normal The surface normal to the plane
    * @param distance Minimum distance from the plane to the origin
    * @param size Dimensions of the plane
    * @param colour What colour to render the body with
    */
    Plane(const glm::vec2& normal,
          float distance,
          float size,
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
    * @return minimum distance from the plane to the origin
    */
    float GetDistance() const;

    /**
    * Sets minimum distance from the plane to the origin
    */
    void SetDistance(float distance);

    /**
    * @return the surface normal to the plane
    */
    const glm::vec2& GetNormal() const;

    /**
    * Sets the surface normal to the plane
    */
    void SetNormal(const glm::vec2& normal);

    /**
    * Sets the surface normal x coordinate to the plane
    */
    void SetNormalX(float x);

    /**
    * Sets the surface normal y coordinate to the plane
    */
    void SetNormalY(float y);

    /**
    * Sets the size of the plane
    */
    void SetSize(float size);

    /**
    * Sets the size of the plane
    */
    float GetSize() const;

private:

    /**
    * Prevent Copying
    */
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;

    float m_distance = 0.0f; ///< Minimum distance from the plane to the origin
    float m_size = 0.0f;     ///< Dimensions of the plane
    glm::vec2 m_normal;      ///< The surface normal to the plane
};