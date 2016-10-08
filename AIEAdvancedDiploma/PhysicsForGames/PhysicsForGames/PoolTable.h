////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PoolTable.h
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

class TutorialData;
class SquareBody;
class CircleBody;

class PoolTable
{
public:

    /**
    * Creates the pool table
    */
    static void Create(TutorialData& data);

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
    */
    static void CreateBoard(TutorialData& data, Actors& actors);

    /**
    * Creates the pool balls
    */
    static void CreateBalls(TutorialData& data, Actors& actors);

    /**
    * Creates the pool cue
    */
    static void CreateCue(TutorialData& data, Actors& actors);
};
