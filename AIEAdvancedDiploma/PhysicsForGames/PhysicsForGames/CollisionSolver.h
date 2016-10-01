////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cpp
////////////////////////////////////////////////////////////////////////////////////////

class PhysicsObject;

class CollisionSolver
{
public:

    /**
    * Solves for collision between two bodies
    */
    bool SolveCollision(PhysicsObject& obj1, PhysicsObject& obj2);

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
};