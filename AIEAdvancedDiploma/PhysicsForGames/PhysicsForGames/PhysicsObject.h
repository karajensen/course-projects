////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsObject.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <functional>
#include <map>

namespace aie
{
    class Renderer2D;
}

class RigidBody;

class PhysicsObject
{
public:

    /**
    * Types of supported physics objects
    */
    enum Shape
    {
        PLANE,
        CIRCLE,
        SQUARE,
        MAX_SHAPES
    };

    /**
    * Types of supported collision responses
    */
    enum CollisionResponse
    {
        NONE,
        STOP,
        COLLIDE
    };

    /**
    * Constructor
    * @param shape The type of object this is
    * @param colour What colour to render the body with
    */
    PhysicsObject(Shape shape, const glm::vec4& colour);
                  
    /**
    * Constructor
    * @param shape The type of object this is
    * @param colour What colour to render the body with
    * @param position The position to initialise the body at
    */
    PhysicsObject(Shape shape, const glm::vec4& colour, const glm::vec2& position);

    /**
    * Destructor
    */
    virtual ~PhysicsObject() {}

    /**
    * Pre-updates the body
    * @param timeStep The time between each update call
    */
    void PreUpdate(float timeStep);

    /**
    * Updates the body
    * @param timeStep The time between each update call
    */
    virtual void Update(float timeStep) = 0;

    /**
    * Post-updates the body
    * @param timeStep The time between each update call
    */
    void PostUpdate(float timeStep);

    /**
    * Renders the body
    * @param renderer The 2D renderer for drawing the body
    */
    virtual void Draw(aie::Renderer2D* renderer);

    /**
    * Outputs debug information about the body
    */
    virtual void Debug();

    /**
    * Applies a force to the body
    */
    virtual void ApplyForce(const glm::vec2& force);

    /**
    * Applies a force to the body
    * @param x/y The components of the force to apply
    */
    virtual void ApplyForce(float x, float y);

    /**
    * Applies a force to the body and then an opposite force to the given actor
    * @param actor The body to apply the opposite force to
    * @param force The force to apply to the body
    */
    virtual void ApplyForceToActor(RigidBody* actor, const glm::vec2& force);

    /**
    * Sets the gravity force
    * @param gravity The gravity to set
    */
    virtual void SetGravity(const glm::vec2& gravity);

    /**
    * Sets the gravity force
    * @param x/y The components of the gravity
    */
    virtual void SetGravity(float x, float y);

    /**
    * Sets the body as active or not
    * @note bodies not active will not take part in the physics scene
    */
    void SetActive(bool active);

    /**
    * @return if the body as active or not
    * @note bodies not active will not take part in the physics scene
    */
    bool IsActive() const;

    /**
    * Sets the body as visible or not
    * @note bodies not visible will not be rendered
    */
    void SetVisible(bool visible);

    /**
    * @return if the body as visible or not
    * @note bodies not visible will not be rendered
    */
    bool IsVisible() const;

    /**
    * Explicitly sets the position of the body
    * @param position The position to set
    */
    void SetPosition(const glm::vec2& position);

    /**
    * Explicitly sets the position of the body
    * @param x/y The components of the position to set
    */
    void SetPosition(float x, float y);

    /**
    * @return the current position of the body
    */
    const glm::vec2& GetPosition() const;

    /**
    * Sets a callback to fire before the body is updated
    */
    void SetPreUpdateFn(std::function<void(float)> fn);

    /**
    * Sets a callback to fire after the body is updated
    */
    void SetPostUpdateFn(std::function<void(float)> fn);

    /**
    * @return the type of physics body this is
    */
    Shape GetShape() const;

    /**
    * Sets that the object is currently in collision this tick
    */
    void SetColliding();

    /**
    * @return whetherthe object is currently in collision this tick
    */
    bool IsColliding() const;

    /**
    * Sets the colour to render the body with
    */
    void SetColor(const glm::vec4& colour);

    /**
    * @return the colour to render the body with
    */
    const glm::vec4& GetColour() const;

    /**
    * Sets whether this body can be collided with
    */
    void SetCollidable(bool isCollidable);

    /**
    * @return whether this body can be collided with
    */
    bool IsCollidable() const;

    /**
    * Sets the type of collision response to perform 
    * when colliding with the object of the given ID
    * @param id The ID of the body colliding with
    * @Param response The type of response to the collision
    */
    void SetCollisionResponse(unsigned int id, CollisionResponse response);

    /**
    * Sets the default type of collision response to perform
    * when colliding with the object of any ID
    */
    void SetCollisionResponse(CollisionResponse response);

    /**
    * @return the type of response to the given object ID
    * @note will default to collide if no response has been set
    */
    CollisionResponse GetCollisionResponse(unsigned int id) const;

    /**
    * @return the unique ID of the body
    */
    unsigned int GetID() const;

protected:

    bool m_isActive = true;    ///< Whether the body is active in the physics world
    bool m_isVisible = true;   ///< Whether the body is visible or not
    glm::vec4 m_colour;        ///< Colour to render the body
    glm::vec2 m_position;      ///< Position of the body
                               
private:

    /**
    * Prevent Copying
    */
    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject& operator=(const PhysicsObject&) = delete;

    const Shape m_shape;                                    ///< The type of physics object this is
    const unsigned int m_id;                                ///< The unique ID of the object
    std::function<void(float)> m_preUpdate = nullptr;       ///< Callback to fire before the body is updated
    std::function<void(float)> m_postUpdate = nullptr;      ///< Callback to fire after the body is updated
    bool m_inCollision = false;                             ///< Whether this object is in collision this tick
    bool m_isCollidable = true;                             ///< Whether this object can be collided with
    std::map<unsigned int, CollisionResponse> m_response;   ///< Type of response when colliding with another object
    CollisionResponse m_defaultResponse = COLLIDE;          ///< Type of response when no id is set
    static unsigned int sm_idCounter;                       ///< Counter for unique physics objects
};