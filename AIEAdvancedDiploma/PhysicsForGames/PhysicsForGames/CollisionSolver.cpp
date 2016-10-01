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

    PhysicsObject::ID shapeID1 = obj1.GetID();
    PhysicsObject::ID shapeID2 = obj2.GetID();

    // using function pointers
    const int index = (shapeID1 * PhysicsObject::MAX_IDS) + shapeID2;
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
    const float distance = glm::length(position1 - position2);

    if (distance <= body1.GetRadius() + body2.GetRadius())
    {
        body1.ResetForces();
        body2.ResetForces();
        body1.SetPosition(body1.GetPreviousPosition());
        body2.SetPosition(body2.GetPreviousPosition());
        return true;
    }
    return false;
}

bool CollisionSolver::SolveSquareSquareCollision(PhysicsObject& square1, PhysicsObject& square2)
{
    return false;
}

bool CollisionSolver::SolveCirclePlaneCollision(PhysicsObject& circle, PhysicsObject& plane)
{
    auto& planeBody = dynamic_cast<Plane&>(plane);
    auto& circleBody = dynamic_cast<CircleBody&>(circle);

    // after casting our collision objects to their correct type we do the following
    glm::vec2 normal = planeBody.GetNormal();
    float sphereToPlane = glm::dot(circle.GetPosition(), normal) - planeBody.GetDistance();

    if (sphereToPlane < 0)
    {
        // if we are behind plane then we flip the normal
        normal *= -1;
        sphereToPlane *= -1;
    }

    float intersection = circleBody.GetRadius() - sphereToPlane;
    if (intersection > 0)
    {
        circleBody.ResetForces();
        circleBody.SetPosition(circleBody.GetPreviousPosition());
        return true;
    }

    return false;
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

