////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SquareBody.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RigidBody.h"

class SquareBody : public RigidBody
{
public:

    /**
    * Constructor
    * @param position The position to initialise the body at
    * @param velocity The initial velocity of the body
    * @param mass The mass of the body
    * @param size The size of the square
    * @param colour What colour to render the body with
    */
    SquareBody(const glm::vec2& position, 
               const glm::vec2& velocity, 
               float mass, 
               const glm::vec2& size,
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
    virtual void Draw(aie::Renderer2D& renderer) override;

    /**
    * Outputs debug information about the body
    */
    virtual void Debug() override;

    /**
    * Sets the size of the square body
    */
    void SetSize(const glm::vec2& size);

    /**
    * Sets the size of the square body
    */
    void SetSize(float x, float y);

    /**
    * @return the size of the square body
    */
    const glm::vec2& GetSize() const;

    /**
    * Sets the radius of the square body
    */
    virtual void SetRadius(float radius) override;

    /**
    * @return the radius of the square body
    */
    virtual float GetRadius() const override;

    /**
    * Creates the position and size of the body from a line
    * @param start The starting point of the line
    * @param end The ending point of the line
    * @param size The width of the line
    */
    void MakeFromLine(const glm::vec2& start, const glm::vec2& end, float size);

private:

    /**
    * Prevent Copying
    */
    SquareBody(const SquareBody&) = delete;
    SquareBody& operator=(const SquareBody&) = delete;

    glm::vec2 m_size; ///< Dimensions of the body
};