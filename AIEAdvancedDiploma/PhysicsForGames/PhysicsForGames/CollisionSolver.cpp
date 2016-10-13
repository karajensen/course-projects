////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionSolver.h"
#include "Tweaker.h"
#include "PhysicsObject.h"
#include "CircleBody.h"
#include "SquareBody.h"
#include "Plane.h"
#include "Utilities.h"
#include "Renderer2D.h"
#include "glm/glm.hpp"
#include <array>

CollisionSolver::CollisionSolver()
{
}

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
    const float radius1 = body1.GetRadius();
    const float radius2 = body2.GetRadius();
    const auto delta = position2 - position1;
    const float distance = glm::length(delta);
    const float intersection = radius1 + radius2 - distance;

    if (intersection <= 0)
    {
        return false;
    }

    const glm::vec2 normal = glm::normalize(delta);
    const glm::vec2 collisionPoint = position1 + (normal * radius1);
    AddDiagnostic(collisionPoint, 0.0f, 1.0f, 1.0f);

    const auto& response1 = circle1.GetCollisionResponse(circle2.GetID());
    const auto& response2 = circle2.GetCollisionResponse(circle1.GetID());
    const bool shouldBody1Respond = response1.first;
    const bool shouldBody2Respond = response2.first;
        
    if (shouldBody1Respond || shouldBody2Respond)
    {
        const glm::vec2 relativeVelocity = body1.GetVelocity() - body2.GetVelocity();
        const float mass = body1.GetMass() + body2.GetMass();
        const float dot = glm::dot(relativeVelocity, normal);
        const float elasticity = (body1.GetElasticity() + body2.GetElasticity()) / 2.0f;
        const glm::vec2 force = (-2.0f / mass) * dot * elasticity * normal;
        const glm::vec2 seperationVector = normal * intersection * 0.5f;

        if (!Utils::IsZero(force))
        {
            if (shouldBody1Respond)
            {
                if (body1.CanRotate())
                {
                    const glm::vec2 axisToCollision = glm::normalize(collisionPoint - position1);
                    const glm::vec2 lever = glm::normalize(glm::vec2(axisToCollision.y, -axisToCollision.x)) * radius1;
                    const float torque = glm::length(Utils::Cross(force, lever)) * (1.0f / body1.GetMomentOfInertia());
                    body1.SetAngularVelocity(torque + body1.GetAngularVelocity());
                }

                if (body1.CanMove())
                {
                    body1.SetVelocity(force + body1.GetVelocity());
                    body1.SetPosition(body1.GetPosition() - seperationVector);
                }
            }

            if (shouldBody2Respond)
            {
                if (body2.CanRotate())
                {
                    const glm::vec2 axisToCollision = glm::normalize(collisionPoint - position2);
                    const glm::vec2 lever = glm::normalize(glm::vec2(axisToCollision.y, -axisToCollision.x)) * radius2;
                    const float torque = glm::length(Utils::Cross(-force, lever)) * (1.0f / body2.GetMomentOfInertia());
                    body2.SetAngularVelocity(-torque + body2.GetAngularVelocity());
                }

                if (body2.CanMove())
                {
                    body2.SetVelocity(-force + body2.GetVelocity());
                    body2.SetPosition(body2.GetPosition() + seperationVector);
                }
            }
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

    const auto& position = circleBody.GetPosition();
    const auto& normal = planeBody.GetNormal();
    glm::vec2 collisionNormal = normal;
    float sphereToPlane = glm::dot(position, normal) - planeBody.GetDistance();
    if (sphereToPlane < 0.0f) 
    { 
        // planes are infintely thin, double sided, objects so if we are behind it we flip the normal 
        collisionNormal *= -1.0f;
        sphereToPlane *= -1.0f;
    } 

    const float radius = circleBody.GetRadius();
    const float intersection = radius - sphereToPlane;
    if (intersection <= 0)
    {
        return false;
    }

    const glm::vec2 collisionPoint = position - (normal * sphereToPlane);
    AddDiagnostic(collisionPoint, 1.0f, 0.0f, 1.0f);

    // Response for plane currently not supported
    const auto& responseCircle = circle.GetCollisionResponse(plane.GetID());
    const bool shouldCircleRespond = responseCircle.first;

    if (shouldCircleRespond)
    {
        const auto& velocity = circleBody.GetVelocity();
        const float dot = glm::dot(normal, velocity);
        const float mass = circleBody.GetMass();
        const float elasticity = circleBody.GetElasticity();
        const glm::vec2 force = (-2.0f / mass) * dot * elasticity * normal;

        if (!Utils::IsZero(force))
        {
            if (circleBody.CanRotate())
            {
                const glm::vec2 axisToCollision = glm::normalize(collisionPoint - position);
                const glm::vec2 lever = glm::normalize(glm::vec2(axisToCollision.y, -axisToCollision.x)) * radius;
                const float torque = glm::length(Utils::Cross(force, lever)) * (1.0f / circleBody.GetMomentOfInertia());
                circleBody.SetAngularVelocity(torque + circleBody.GetAngularVelocity());
            }

            if (circleBody.CanMove())
            {
                circleBody.SetVelocity(force + velocity);
                circleBody.SetPosition(circleBody.GetPosition() + normal * intersection * 0.5f);
            }
        }
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

        const float radius = circleBody.GetRadius();
        const float intersection = radius - sphereToPlane;
        if (intersection <= 0)
        {
            return false;
        }

        const glm::vec2 collisionPoint = position - (normal * sphereToPlane);
        if (glm::length(center - collisionPoint) > distance * 0.5f)
        {
            return false;
        }

        AddDiagnostic(collisionPoint, 1.0f, 0.0f, 1.0f);

        // Response for square currently not supported
        const auto& responseCircle = circleBody.GetCollisionResponse(squareBody.GetID());
        const bool shouldCircleRespond = responseCircle.first;

        if (shouldCircleRespond)
        {
            const auto& velocity = circleBody.GetVelocity();
            const float dot = glm::dot(normal, velocity);
            const float mass = circleBody.GetMass();
            const float elasticity = circleBody.GetElasticity();
            const glm::vec2 force = (-2.0f / mass) * dot * elasticity * normal;
            
            if (!Utils::IsZero(force))
            {
                if (circleBody.CanRotate())
                {
                    const glm::vec2 axisToCollision = glm::normalize(collisionPoint - position);
                    const glm::vec2 lever = glm::normalize(glm::vec2(axisToCollision.y, -axisToCollision.x)) * radius;
                    const float test = glm::length(Utils::Cross(force, lever));
                    const float test2 = circleBody.GetMomentOfInertia();
                    const float torque = glm::length(Utils::Cross(force, lever)) * (1.0f / circleBody.GetMomentOfInertia());
                    circleBody.SetAngularVelocity(torque + circleBody.GetAngularVelocity());
                }

                if (circleBody.CanMove())
                {
                    circleBody.SetVelocity(force + velocity);
                    circleBody.SetPosition(position + normal * intersection * 0.5f);
                }
            }
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
    const float size = 7.5f;
    for (auto itr = m_diagnostics.begin(); itr != m_diagnostics.end(); )
    {
        auto& data = itr->second;
        renderer.setRenderColour(data.first.r, data.first.g, data.first.b, 1.0f);
        renderer.drawBox(itr->first.first, itr->first.second, size, size);

        --data.second;
        if (data.second <= 0)
        {
            m_diagnostics.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
}

void CollisionSolver::AddDiagnostic(const glm::vec2& point, float r, float g, float b)
{
    if (m_renderDiagnostics)
    {
        m_diagnostics[std::make_pair(point.x, point.y)] = std::make_pair(glm::vec3(r, g, b), 75);
    }
}

void CollisionSolver::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddBoolEntry("Collision Diagnosics",
        [this]() { return m_renderDiagnostics; },
        [this](bool value) { m_renderDiagnostics = value;  });
}