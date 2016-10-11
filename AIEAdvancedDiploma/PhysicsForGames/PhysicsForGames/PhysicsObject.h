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

/**
* Physics body base class for all actors
*/
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
    * Constructor
    * @param shape The type of object this is
    * @param colour What colour to render the body with
    */
    PhysicsObject(Shape shape, const glm::vec4& colour);

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
    * Update to be done when the body is inactive
    * @param timeStep The time between each update call
    */
    void InactiveUpdate();

    /**
    * Renders the body
    * @param renderer The 2D renderer for drawing the body
    * @param showDiagnostics Whether to render diagnostics
    */
    virtual void Draw(aie::Renderer2D& renderer, bool showDiagnostics);

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
    * Sets a callback to fire before the body is updated
    */
    void SetPreUpdateFn(std::function<void(float)> fn);

    /**
    * Sets a callback to fire after the body is updated
    */
    void SetPostUpdateFn(std::function<void(float)> fn);

    /**
    * Sets a callback to fire after the body is inactive
    */
    void SetInactiveFn(std::function<void(void)> fn);

    /**
    * @return the type of physics body this is
    */
    Shape GetShape() const;

    /**
    * Sets the colour to render the body with
    */
    void SetColor(const glm::vec4& colour);

    /**
    * Sets the colour to render the body with
    */
    void SetColor(float r, float g, float b, float a);

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
    * Sets that the object is currently in collision this tick
    */
    void SetColliding();

    /**
    * @return whetherthe object is currently in collision this tick
    */
    bool IsColliding() const;

    /**
    * Sets the type of collision response to perform 
    * when colliding with the object of the given ID
    * @param id The ID of the body colliding with
    * @Param shouldCollide Whether the body should respond with force
    * @param fn An optional callback to trigger on collision
    */
    void SetCollisionResponse(unsigned int id,
                              bool shouldCollide,
                              std::function<void(void)> fn = nullptr);

    /**
    * Sets the type of collision response to perform
    * @Param shouldCollide Whether the body should respond with force
    * @param fn An optional callback to trigger on collision
    */
    void SetCollisionResponse(bool shouldCollide,
                              std::function<void(void)> fn = nullptr);

    /**
    * @return the type of response to the given object ID
    * @note will default to collide if no response has been set
    */
    typedef std::pair<bool, std::function<void(void)>> CollisionResponse;
    const CollisionResponse& GetCollisionResponse(unsigned int id) const;

    /**
    * @return the unique ID of the body
    */
    unsigned int GetID() const;

protected:

    bool m_isActive = true;    ///< Whether the body is active in the physics world
    bool m_isVisible = true;   ///< Whether the body is visible or not
    glm::vec4 m_colour;        ///< Colour to render the body
                               
private:

    /**
    * Prevent Copying
    */
    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject& operator=(const PhysicsObject&) = delete;

    const Shape m_shape;                                    ///< The type of physics object this is
    const unsigned int m_id;                                ///< The unique ID of the object
    std::function<void(void)> m_inactiveFn = nullptr;       ///< Callback to fire when inactive
    std::function<void(float)> m_preUpdate = nullptr;       ///< Callback to fire before the body is updated
    std::function<void(float)> m_postUpdate = nullptr;      ///< Callback to fire after the body is updated
    bool m_inCollision = false;                             ///< Whether this object is in collision this tick
    bool m_isCollidable = true;                             ///< Whether this object can be collided with
    std::map<unsigned int, CollisionResponse> m_response;   ///< Type of response when colliding with another object
    CollisionResponse m_defaultResponse;                    ///< Type of response when no id is set
    static unsigned int sm_idCounter;                       ///< Counter for unique physics objects
};