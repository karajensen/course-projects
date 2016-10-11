////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <map>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class PhysicsObject;
class Tweaker;

namespace aie
{
    class Renderer2D;
}

/**
* Detects and resolves collision between two physics actors
*/
class CollisionSolver
{
public:

    /**
    * Constructor
    */
    CollisionSolver();

    /**
    * Solves for collision between two bodies
    */
    bool SolveCollision(PhysicsObject& obj1, PhysicsObject& obj2);

    /**
    * Renders the diagnostics
    * @param renderer The 2D renderer for drawing
    */
    void Draw(aie::Renderer2D& renderer);

    /**
    * Adds the collision solver to the tweakbar
    */
    void AddToTweaker(Tweaker& tweaker);

private:

    /**
    * Solves collision between two planes
    * @param plane1 The first plane taking part in the collision
    * @param plane2 The second plane taking part in the collision
    */
    bool SolvePlanePlaneCollision(PhysicsObject& plane1, PhysicsObject& plane2);

    /**
    * Solves collision between a circle and circle
    * @param circle1 The first circle taking part in the collision
    * @param circle2 The second circle taking part in the collision
    */
    bool SolveCircleCircleCollision(PhysicsObject& circle1, PhysicsObject& circle2);

    /**
    * Solves collision between a square and square
    * @param square1 The first square taking part in the collision
    * @param square2 The second square taking part in the collision
    */
    bool SolveSquareSquareCollision(PhysicsObject& square1, PhysicsObject& square2);

    /**
    * Solves collision between a plane and circle
    * @param plane The plane taking part in the collision
    * @param circle The circle body taking part in the collision
    */
    bool SolvePlaneCircleCollision(PhysicsObject& plane, PhysicsObject& circle);
    bool SolveCirclePlaneCollision(PhysicsObject& circle, PhysicsObject& plane);

    /**
    * Solves collision between a square and circle
    * @param square The square body taking part in the collision
    * @param circle The circle body taking part in the collision
    */
    bool SolveSquareCircleCollision(PhysicsObject& square, PhysicsObject& circle);
    bool SolveCircleSquareCollision(PhysicsObject& circle, PhysicsObject& square);

    /**
    * Solves collision between a square and plane
    * @param square The square body taking part in the collision
    * @param plane The plane taking part in the collision
    */
    bool SolveSquarePlaneCollision(PhysicsObject& square, PhysicsObject& plane);
    bool SolvePlaneSquareCollision(PhysicsObject& plane, PhysicsObject& square);

    /**
    * Adds a point to the diagnostics
    * @param point The position to add at
    * @param r/g/b The colour to add
    */
    void AddDiagnostic(const glm::vec2& point, float r, float g, float b);

    typedef std::pair<glm::vec3, int> DiagnosticData;
    typedef std::pair<float, float> DiagnosticKey;
    std::map<DiagnosticKey, DiagnosticData> m_diagnostics;   ///< Points to render for diagnostics
    bool m_renderDiagnostics = false;                        ///< Whether to render diagnostics
};