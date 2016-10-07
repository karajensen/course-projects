////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionSolver.h"
#include "PhysicsObject.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "Renderer2D.h"
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

    const auto& position1 = body1.GetPosition();
    const auto& position2 = body2.GetPosition();
    const auto delta = position2 - position1;
    const float distance = glm::length(delta);
    const float intersection = body1.GetRadius() + body2.GetRadius() - distance;

    if (intersection <= 0)
    {
        return false;
    }

    const auto& response1 = circle1.GetCollisionResponse(circle2.GetID());
    const auto& response2 = circle2.GetCollisionResponse(circle1.GetID());
        
    if (response1.first || response2.first)
    {
        const glm::vec2 collisionNormal = glm::normalize(delta);
        const glm::vec2 relativeVelocity = body1.GetVelocity() - body2.GetVelocity();
        const glm::vec2 collisionVector = collisionNormal * glm::dot(relativeVelocity, collisionNormal);
        const glm::vec2 force = 2.0f * (collisionVector * 1.0f / (1.0f / body1.GetMass() + 1.0f / body2.GetMass()));
        const glm::vec2 seperationVector = collisionNormal * intersection * 0.5f;

        if (response1.first)
        {
            body1.ApplyForce(-force);
            body1.SetPosition(body1.GetPosition() - seperationVector);
        }

        if (response2.first)
        {
            body2.ApplyForce(force);
            body2.SetPosition(body2.GetPosition() + seperationVector);
        }
    }

    if (response1.second)
    {
        response1.second();
    }

    if (response2.second)
    {
        response2.second();
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
    const auto& responseCircle = circle.GetCollisionResponse(plane.GetID());
    if (responseCircle.first)
    {
        const auto& velocity = circleBody.GetVelocity();
        const float dot = glm::dot(normal, velocity);
        const float mass = std::max(circleBody.GetMass(), 0.001f);
        const glm::vec2 reflection = (-2.0f / mass) * dot * normal + velocity;
        circleBody.SetVelocity(reflection);
        circleBody.SetPosition(circleBody.GetPosition() + normal * intersection * 0.5f);
    }

    if (responseCircle.second)
    {
        responseCircle.second();
    }

    return true; 
}

bool CollisionSolver::SolveCircleSquareCollision(PhysicsObject& circle, PhysicsObject& square)
{
    auto& squareBody = dynamic_cast<SquareBody&>(square);
    auto& circleBody = dynamic_cast<CircleBody&>(circle);

    auto testSquareSide = [this, &squareBody, &circleBody](const glm::vec2& c1, const glm::vec2& c2) -> bool
    {
        const glm::vec2 direction = c2 - c1;
        const glm::vec2 center = c1 + (direction * 0.5f);
        const float distance = glm::length(direction);
        const glm::vec2 normal = -glm::normalize(glm::vec2(direction.y, -direction.x));
        const auto& position = circleBody.GetPosition();

        float sphereToPlane = glm::dot(position - c1, normal);
        if (sphereToPlane < 0.0f)
        {
            return false;
        }

        const float intersection = circleBody.GetRadius() - sphereToPlane;
        if (intersection <= 0)
        {
            return false;
        }

        const glm::vec2 collisionPoint = position - (normal * sphereToPlane);
        if (glm::length(center - collisionPoint) > distance * 0.5f)
        {
            return false;
        }

        // Response for square currently not supported
        const auto& responseCircle = circleBody.GetCollisionResponse(squareBody.GetID());
        if (responseCircle.first)
        {
            const auto& velocity = circleBody.GetVelocity();
            const float dot = glm::dot(normal, velocity);
            const float mass = std::max(circleBody.GetMass(), 0.001f);
            const glm::vec2 reflection = (-2.0f / mass) * dot * normal + velocity;
            circleBody.SetVelocity(reflection);
            circleBody.SetPosition(position + normal * intersection * 0.5f);
        }

        if (responseCircle.second)
        {
            responseCircle.second();
        }

        return true;
    };

    const glm::vec2& position = squareBody.GetPosition();
    const glm::vec2& size = squareBody.GetSize();
    const glm::vec2 halfSize = size * 0.5f;
    const glm::vec2 topLeft(position.x - halfSize.x, position.y + halfSize.y);
    const glm::vec2 topRight(position.x + halfSize.x, position.y + halfSize.y);
    const glm::vec2 botLeft(position.x - halfSize.x, position.y - halfSize.y);
    const glm::vec2 botRight(position.x + halfSize.x, position.y - halfSize.y);

    return testSquareSide(topLeft, topRight)  ||
           testSquareSide(topRight, botRight) ||
           testSquareSide(botRight, botLeft)  ||
           testSquareSide(botLeft, topLeft);
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

void CollisionSolver::Draw(aie::Renderer2D& renderer)
{
    if (!m_diagnostics.empty())
    {
        const float size = 10.0f;

        for (const auto& pair : m_diagnostics)
        {
            renderer.setRenderColour(pair.second.r, pair.second.g, pair.second.b, 1.0f);
            renderer.drawBox(pair.first.x, pair.first.y, size, size);
        }

        m_diagnostics.clear();
    }
}

void CollisionSolver::AddDiagnostic(const glm::vec2& point, float r, float g, float b)
{
    m_diagnostics.push_back(std::make_pair(point, glm::vec3(r, g, b)));
}