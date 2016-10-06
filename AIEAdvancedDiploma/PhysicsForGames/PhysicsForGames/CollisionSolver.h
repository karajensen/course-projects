////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class PhysicsObject;

namespace aie
{
    class Renderer2D;
}

class CollisionSolver
{
public:

    /**
    * Solves for collision between two bodies
    */
    bool SolveCollision(PhysicsObject& obj1, PhysicsObject& obj2);

    /**
    * Renders the diagnostics
    * @param renderer The 2D renderer for drawing
    */
    void Draw(aie::Renderer2D& renderer);

private:

    /**
    * Solves collision between two planes
    */
    bool SolvePlanePlaneCollision(PhysicsObject& plane1, PhysicsObject& plane2);

    /**
    * Solves collision between a circle and circle
    */
    bool SolveCircleCircleCollision(PhysicsObject& circle1, PhysicsObject& circle2);

    /**
    * Solves collision between a square and square
    */
    bool SolveSquareSquareCollision(PhysicsObject& square1, PhysicsObject& square2);

    /**
    * Solves collision between a plane and circle
    */
    bool SolvePlaneCircleCollision(PhysicsObject& plane, PhysicsObject& circle);
    bool SolveCirclePlaneCollision(PhysicsObject& circle, PhysicsObject& plane);

    /**
    * Solves collision between a square and circle
    */
    bool SolveSquareCircleCollision(PhysicsObject& square, PhysicsObject& circle);
    bool SolveCircleSquareCollision(PhysicsObject& circle, PhysicsObject& square);

    /**
    * Solves collision between a square and plane
    */
    bool SolveSquarePlaneCollision(PhysicsObject& square, PhysicsObject& plane);
    bool SolvePlaneSquareCollision(PhysicsObject& plane, PhysicsObject& square);

    /**
    * Adds a point to the diagnostics
    */
    void AddDiagnostic(const glm::vec2& point, float r, float g, float b);

    std::vector<std::pair<glm::vec2, glm::vec3>> m_diagnostics; ///< Points to render for diagnostics
};