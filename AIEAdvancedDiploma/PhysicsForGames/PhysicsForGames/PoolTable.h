////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PoolTable.h
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

class TutorialData;
class SquareBody;
class CircleBody;
class RigidBody;

/**
* Tutorial component for generating a pool table
*/
class PoolTable
{
public:

    /**
    * Creates the pool table
    * @param data For creating the tutorial
    * @param useLinearDrag whether the balls should use linear drag
    * @param useRotation whether the balls should rotate
    * @param useElasticity whether the balls should use elasticity
    */
    static void Create(TutorialData& data,
                       bool useLinearDrag,
                       bool useRotation,
                       bool useElastcity);

private:

    /**
    * Actors created for the table
    */
    struct Actors
    {
        SquareBody* center = nullptr;       ///< Center backdrop of table
        std::vector<SquareBody*> barriers;  ///< Barrier backdrop of table
        std::vector<CircleBody*> pockets;   ///< Pocket bodies of table
        std::vector<CircleBody*> balls;     ///< Balls of the table
        CircleBody* playerBall = nullptr;   ///< Ball player can interact with
    };

    /**
    * Creates the table board
    * @param data For creating the tutorial data
    * @param actors Cached information for physics scene actors created
    */
    static void CreateBoard(TutorialData& data, Actors& actors);

    /**
    * Creates the pool balls
    * @param data For creating the tutorial data
    * @param actors Cached information for physics scene actors created
    * @param useRotation whether the balls should rotate
    */
    static void CreateBalls(TutorialData& data, Actors& actors, bool useRotation);

    /**
    * Creates the pool cue
    * @param data For creating the tutorial data
    * @param actors Cached information for physics scene actors created
    */
    static void CreateCue(TutorialData& data, Actors& actors);
};
