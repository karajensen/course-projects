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
    virtual void Draw(aie::Renderer2D* renderer) override;

    /**
    * Outputs debug information about the body
    */
    virtual void Debug() override;

private:

    /**
    * Prevent Copying
    */
    SquareBody(const SquareBody&) = delete;
    SquareBody& operator=(const SquareBody&) = delete;

    glm::vec2 m_size;   ///< Dimensions of the body
};