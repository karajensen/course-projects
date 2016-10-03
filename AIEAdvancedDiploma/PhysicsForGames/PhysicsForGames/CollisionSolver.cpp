////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionSolver.h"
#include "PhysicsObject.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "glm/glm.hpp"
#include <array>

bool CollisionSolver::SolveCollision(PhysicsObject& obj1, PhysicsObject& obj2)
{
    // function pointer array for doing our collisions
    typedef bool(CollisionSolver::*CollisionFn)(PhysicsObject&, PhysicsObject&);

    static std::array<CollisionFn, 9> collisionfunctionArray =
    {
        &CollisionSolver::SolvePlanePlaneCollision,
        &CollisionSolver::SolvePlaneCircleCollision,
        &CollisionSolver::SolvePlaneSquareCollision,
        &CollisionSolver::SolveCirclePlaneCollision,
        &CollisionSolver::SolveCircleCircleCollision,
        &CollisionSolver::SolveCircleSquareCollision,
        &CollisionSolver::SolveSquarePlaneCollision,
        &CollisionSolver::SolveSquareCircleCollision,
        &CollisionSolver::SolveSquareSquareCollision,
    };

    if (!obj1.IsCollidable() || !obj2.IsCollidable())
    {
        return false;
    }

    PhysicsObject::Shape shape1 = obj1.GetShape();
    PhysicsObject::Shape shape2 = obj2.GetShape();

    // using function pointers
    const int index = (shape1 * PhysicsObject::MAX_SHAPES) + shape2;
    assert(index >= 0 && index < (int)collisionfunctionArray.size());

    CollisionFn collisionFunctionPtr = collisionfunctionArray[index];
    if ((this->*collisionFunctionPtr)(obj1, obj2))
    {
        obj1.SetColliding();
        obj2.SetColliding();
        return true;
    }
    return false;
}

bool CollisionSolver::SolvePlanePlaneCollision(PhysicsObject& plane1, PhysicsObject& plane2)
{
    return false;
}

bool CollisionSolver::SolveCircleCircleCollision(PhysicsObject& circle1, PhysicsObject& circle2)
{
    auto& body1 = dynamic_cast<CircleBody&>(circle1);
    auto& body2 = dynamic_cast<CircleBody&>(circle2);

    const auto& position1 = circle1.GetPosition();
    const auto& position2 = circle2.GetPosition();
    const auto delta = position2 - position1;
    const float distance = glm::length(delta);
    const float intersection = body1.GetRadius() + body2.GetRadius() - distance;

    if (intersection <= 0)
    {
        return false;
    }

    const auto response1 = circle1.GetCollisionResponse(circle2.GetID());
    const auto response2 = circle2.GetCollisionResponse(circle1.GetID());
        
    if (response1 == PhysicsObject::COLLIDE || response2 == PhysicsObject::COLLIDE)
    {
        const glm::vec2 collisionNormal = glm::normalize(delta);
        const glm::vec2 relativeVelocity = body1.GetVelocity() - body2.GetVelocity();
        const glm::vec2 collisionVector = collisionNormal * glm::dot(relativeVelocity, collisionNormal);
        const glm::vec2 force = 2.0f * (collisionVector * 1.0f / (1.0f / body1.GetMass() + 1.0f / body2.GetMass()));
        const glm::vec2 seperationVector = collisionNormal * intersection * 0.5f;

        if (response1 == PhysicsObject::COLLIDE)
        {
            body1.ApplyForce(-force);
            body1.SetPosition(body1.GetPosition() - seperationVector);
        }

        if (response2 == PhysicsObject::COLLIDE)
        {
            body2.ApplyForce(force);
            body2.SetPosition(body2.GetPosition() + seperationVector);
        }
    }

    if (response1 == PhysicsObject::STOP)
    {
        body1.ResetVelocity();
        body1.SetPosition(body1.GetPreviousPosition());
    }

    if (response2 == PhysicsObject::STOP)
    {
        body2.ResetForces();
        body2.SetPosition(body2.GetPreviousPosition());
    }

    return true;
}

bool CollisionSolver::SolveSquareSquareCollision(PhysicsObject& square1, PhysicsObject& square2)
{
    return false;
}

bool CollisionSolver::SolveCirclePlaneCollision(PhysicsObject& circle, PhysicsObject& plane)
{
    auto& planeBody = dynamic_cast<Plane&>(plane);
    auto& circleBody = dynamic_cast<CircleBody&>(circle);

    const auto& normal = planeBody.GetNormal();
    glm::vec2 collisionNormal = normal;
    float sphereToPlane = glm::dot(circleBody.GetPosition(), normal) - planeBody.GetDistance();
    if (sphereToPlane < 0.0f) 
    { 
        // planes are infintely thin, double sided, objects so if we are behind it we flip the normal 
        collisionNormal *= -1.0f;
        sphereToPlane *= -1.0f;
    } 

    const float intersection = circleBody.GetRadius() - sphereToPlane;
    if (intersection <= 0)
    {
        return false;
    }

    // Response for plane currently not supported
    const auto responseCircle = circle.GetCollisionResponse(plane.GetID());
    if (responseCircle == PhysicsObject::COLLIDE)
    {
        const glm::vec2 forceVector = -1.0f * circleBody.GetMass() *
            collisionNormal * (glm::dot(collisionNormal, circleBody.GetVelocity()));

        circleBody.ApplyForce(2.0f * forceVector);
        circleBody.SetPosition(circleBody.GetPosition() + collisionNormal * intersection * 0.5f);
    }
    else if (responseCircle == PhysicsObject::STOP)
    {
        circleBody.ResetForces();
        circleBody.SetPosition(circleBody.GetPreviousPosition());
    }

    return true; 
}

bool CollisionSolver::SolveCircleSquareCollision(PhysicsObject& circle, PhysicsObject& square)
{
    return false;
}

bool CollisionSolver::SolvePlaneSquareCollision(PhysicsObject& plane, PhysicsObject& square)
{
    return false;
}

bool CollisionSolver::SolveSquareCircleCollision(PhysicsObject& square, PhysicsObject& circle)
{
    return SolveCircleSquareCollision(circle, square);
}

bool CollisionSolver::SolvePlaneCircleCollision(PhysicsObject& plane, PhysicsObject& circle)
{
    return SolveCirclePlaneCollision(circle, plane);
}

bool CollisionSolver::SolveSquarePlaneCollision(PhysicsObject& square, PhysicsObject& plane)
{
    return SolvePlaneSquareCollision(plane, square);
}

