// * ************************************************************************ //
// * * MyBot Header                                                       * * //
// * *   : defines Controller Implementation                              * * //
// * *                                                                    * * //
// * * Disclaimer: This document represents no significant intellectual   * * //
// * * property and my be used freely. The author(s) takes no             * * //
// * * responsibility for any loss or damage arising from the use of      * * //
// * * the computer code contained herein. Where this document is         * * //
// * * submitted as part of an assessment task this header must remain    * * //
// * * intact and the student must make clear indication which parts      * * //
// * * have been added by themselves.                                     * * //
// * *                                                                    * * //
// * ********************************************************************** * //
// * ********************************************************************** * //
// * * MyBot.h                                                            * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef MYBOT_H
#define MYBOT_H
#include "MyBotStructs.h"

//COMMANDS
//z = FORWARD, RIGHT, BACK, LEFT [p.view[z] == y]
//y = EMPTY, FOG, WALL, HUNTER, SHROOM, MAGICSHROOM 
//x = QUIT, STEP, TURNLEFT, TURNRIGHT, LOOK, EAT [a = x]

class MyBot : public IController
{
	private:
		LPCONTROLLABLE	m_pAvatar;

	public:
		MyBot(){};
		~MyBot(){};
		virtual char const* name() const;
		virtual char const* id() const;
		virtual void Attach(LPCONTROLLABLE const& pObj);
		virtual void OnStart(CONFIG_PARAMS const *pGameParams);
		virtual void OnTick(const PERCEPT &p);
		virtual void OnStop();

		void SlowBot(clock_t delay);
		bool OnWorldEntry(const PERCEPT &p);
		void GenerateActions(const PERCEPT &p);
		void CreateSurroundingNodes(const PERCEPT &p);
		void CreateNewNode(int type, int x, int y);
		void UpdateHunterFromLastTick();
		int ChangeOrientation(int currentOrientation, int direction);
		act  GetNextAction();
		void PopNextAction();
		square FindCoordinates(int SearchDirection, int SpacesAway);
		square FindCoordinates(int SearchDirection, int SpacesAway, square & position, int direction = -100);
		node* FindNode(int SearchDirection, int SpacesAway); //returns NULL if doesn't exist
		node* FindNode(square & coordinates);
		bool NodeExists(square & coordinates);
		bool NodeExists(string key);
		void ClearAllQueues();

		//mushroooms
		bool CheckForTraps(const PERCEPT &p);
		bool SquareIsMushroom(square & coord);
		bool CheckMagicShroom(const PERCEPT &p, bool eat);
		void CheckMushrooms(const PERCEPT &p);
		void AddToPriorityQueue(act newAction);
		void CalculateMushroomProbability();
		void EatMushroom(node* ShroomToRemove);
		void UpdateMushroomActualGain();

		//walls
		void CheckWalls(const PERCEPT &p);
		bool SquareIsWall(int x, int y);
		bool SquareIsWall(square & coordinates); 
		bool SquareIsWall(node & n);
		bool SafetyNet();

		//paths
		int PathChosen;
		void InnerSpiral(const PERCEPT &p, bool reset);
		void ZigZag(const PERCEPT &p, bool reset);
		void ZigZagFinal(const PERCEPT &p, bool reset);
		void OuterSpiral(const PERCEPT &p, bool reset);
		void Prediction(const PERCEPT &p, bool reset);
		void CleanUp(const PERCEPT &p, bool reset);

		//dynamic path tools
		deque<edge> & MyBot::CreateOutgoingEdges(NodeStruct & currentTile, deque<edge> & edgesToProcess);
		bool FindBestPath(square & Target, ActionQueue & Q);
		float CreateHeuristic(square & coord, square & target);
		bool TargetObscured;

		//zigzag tools
		void ZigZagFinalTurn();

		//cleanup tools
		square CleanUpTarget;
		int CleanUpDepth;
		ActionQueue OQ;
		bool MoveTowardsNearestBlankNode(int cleanDepth);
		bool SearchForEmptySquare(int direction, SquareContainer & SearchedSqrs, int SearchDepth);
		bool OldCleanupPath(square & Target, ActionQueue & Q);

		//MSD
		static bool SaveMSD;
		static bool UseSavedMSD;
		static MagicShroomContainer MSD;
		static int TotalMSfound;
		void MagicShroomProbability();
		int FindDistanceBand(square & coord);
		void SaveMagicShroom(square & coord);
		float GetSquareValue(square & coord);
		void LoadMSD();
		void SaveMSDtoFile();

		//Prediction
		float BestChoice;
		float LastBestChoice;
		ActionQueue EQ;
		square PredictionTarget;
		unsigned int SearchIndex;
		bool FindPredictionPath();
		bool SearchForTarget();
		bool SearchForBestSquare(int direction, SquareContainer & SearchedSqrs, int SearchDepth, float valueToFind);
			
		//tools
		string K(int x, int y); //generate key given coordinates
		string K(square & x);
		float M(vec2D & v); //find magnitude of a vector
		float M(int x, int y);
		stringstream ss;
		static int trialNo;

		//Mushrooms
		bool WantMushrooms;
		bool DynamicPath;
		int MushroomTypeNo;
		unsigned int CurrentMushroomNo;
		unsigned int OverallMushroomNo;
		unsigned int MinMushroomNo;
		float ExpectedGain; //overall gain if all mushrooms eated
		float ActualGain; //gain so far
		float LikelyGain; //likely gain for next mushroom
		MushroomSpread MushroomDistribution;
		node* MagicShroom;

		//Hunter
		square Pos;
		square LastPos;
		int Orientation;
		int StepCost;
		int TurnCost;
		int LookCost;
		int LastHealth;
		int Health;
		int MinimumHealth;
		act NextAction;
		act LastAction;

		//World
		bool FirstTick;
		int WorldColumns;
		int WorldRows;
		int MapColumns;
		int MapRows;
		ShroomMap Map;
		ActionQueue DQ; //direction queue
		ActionQueue PQ; //priority queue
		Wall MapWalls;
		int MapNo;
};

#endif

// ************************************************************************** //
