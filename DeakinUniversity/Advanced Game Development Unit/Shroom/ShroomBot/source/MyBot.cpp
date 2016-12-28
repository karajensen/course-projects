// * ************************************************************************ //
// * * MyBot source                                                       * * //
// * *   : defines MyBot Implementation                                   * * //
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
// * * MyBot.cpp                                                          * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "MyBot.h"

//-----------------------------------------------------------------------------------------------
//START
//-----------------------------------------------------------------------------------------------
void MyBot::OnStart(CONFIG_PARAMS const *pGameParams)
{
	//HUNTER
	StepCost = pGameParams->hunter_cost_step; 
	TurnCost = pGameParams->hunter_cost_turn;
	LookCost = pGameParams->hunter_cost_look;
	Health = pGameParams->hunter_health;
	LastHealth = Health;
	MinimumHealth = 10;
	Orientation = oUP;
	Pos.x = 0;
	Pos.y = 0;
	LastPos = Pos;
	TargetObscured = false;

	//GAME
	//srand((unsigned int) time(NULL));
	FirstTick = true;
	WorldColumns = pGameParams->world_cols;
	WorldRows = pGameParams->world_rows;
	MapColumns = WorldColumns-2;
	MapRows = WorldRows-2;
	MapWalls.FoundAllWalls = false;
	MapWalls.UpWallReached = false;
	MapWalls.DownWallReached = false;
	MapWalls.LeftWallReached = false;
	MapWalls.RightWallReached = false;
	MapWalls.topleft.x = MapWalls.bottomright.x = 0;
	MapWalls.topleft.y = MapWalls.bottomright.y = 0;
	trialNo++;

	//MUSHROOMS
	MagicShroom = NULL;
	MushroomTypeNo = pGameParams->shroom_types;
	CurrentMushroomNo = pGameParams->shroom_num;
	OverallMushroomNo = pGameParams->shroom_num;
	MinMushroomNo = OverallMushroomNo/3;
	short * tempCount = pGameParams->shroom_count;
	short * tempValue = pGameParams->shroom_value;
	for(int i = 0; i < MushroomTypeNo; i++)
	{
		mushType temp;
		temp.value = *(tempValue + i);
		temp.count = *(tempCount + i);
		MushroomDistribution.push_back(temp);
	}
	CalculateMushroomProbability();
	if(ExpectedGain < 0)
		WantMushrooms = false;
	else
		WantMushrooms = true;

	//CHOOSE MAP
	if((OverallMushroomNo == 300)||((WorldColumns == 47)&&(WorldRows == 22)))
		MapNo = MAP3;
	if((OverallMushroomNo == 10)||((WorldColumns == 25)&&(WorldRows == 35)))
		MapNo = MAP2;	
	if((OverallMushroomNo == 25)||((WorldColumns == 25)&&(WorldRows == 25)))
		MapNo = MAP1;
	DynamicPath = false;
	CleanUpDepth = 1;

	if(MapNo == MAP1)
		PathChosen = ZIGZAG;
	else if(MapNo == MAP2)
		PathChosen = ZIGZAGFINAL;		
	else if(MapNo == MAP3)
		PathChosen = ZIGZAGFINAL;

	//MAGIC SHROOM DISTRIBUTION			
	//LoadMSD();
}
//-----------------------------------------------------------------------------------------------
//ONTICK
//-----------------------------------------------------------------------------------------------
void MyBot::OnTick(const PERCEPT &p)
{
	//BOT SPEED
	//SlowBot(50);

	if(trialNo >= 999)
		int x = 0;

	//ON ENTRY
	if(FirstTick)
		FirstTick = OnWorldEntry(p);

	//HEALTH
	Health = p.health;
	if(MapNo == MAP3)
	{
		if(Health < MinimumHealth)
			WantMushrooms = true;
		else
			WantMushrooms = false;
	}

	//HUNTER
	UpdateHunterFromLastTick();

	//UPDATE MUSHROOMS
	if(LastAction.action == EAT)
		UpdateMushroomActualGain();

	//CREATE NEW NODES
	if(LastAction.action == STEP)
		CreateSurroundingNodes(p);

	//MAGIC SHROOM
	if(CheckMagicShroom(p,true))
		return;

	//CHECK WALLS
	CheckWalls(p);

	//DIRECTION
	GenerateActions(p);

	//EAT MUSHROOMS
	NextAction = GetNextAction(); //peek at next action
	if(NextAction.action != EAT)
		CheckMushrooms(p);

	//ACTION
	NextAction = GetNextAction(); //in case of updates
	PopNextAction();
	LastHealth = Health;
	m_pAvatar->ProcessAction(NextAction.action);
	return;
}

//-----------------------------------------------------------------------------------------------
//STOP
//-----------------------------------------------------------------------------------------------
void MyBot::OnStop()
{
	LastAction.action = NextAction.action = QUIT;
	DQ.clear();
	PQ.clear();
	OQ.clear();
	Map.clear();
	MushroomDistribution.clear();
}
