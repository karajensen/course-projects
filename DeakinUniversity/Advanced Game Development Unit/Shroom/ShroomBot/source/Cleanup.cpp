#include "MyBot.h"
//-----------------------------------------------------------------------------------------------
//CLEANUP
//-----------------------------------------------------------------------------------------------
void MyBot::CleanUp(const PERCEPT &p, bool reset)
{
	if(reset || TargetObscured)
	{
		OQ.clear();
		TargetObscured = false;
	}

	if(!MoveTowardsNearestBlankNode(CleanUpDepth))
	{
		//All else fails, just randomly choose!
		int r = rand() % 6; //0 to 5
		if(r <= 3)
			DQ.push_back(act(STEP));
		else if(r == 4)
			DQ.push_back(act(TURNRIGHT));
		else if(r == 5)
			DQ.push_back(act(TURNLEFT));
	}
}
//-----------------------------------------------------------------------------------------------
//Find the blank nodes
//-----------------------------------------------------------------------------------------------
bool MyBot::MoveTowardsNearestBlankNode(int cleanDepth)
{

	//If need new target
	while(OQ.empty())
	{
		SquareContainer SearchedSqrs;
		int SearchDepth = cleanDepth;
		int MaxSearchDepth = max(MapColumns,MapRows);
		bool Found = false;
		square vPos;

		while(!Found)
		{
			//randomly choose over all four directions
			int index;
			vector<int> tempIndex;
			for(int i = 0; i < 4; i++)
			{
				bool okay = false;
				while(!okay)
				{
					okay = true;
					index = rand() % 4; //from 0 to 3
					for(unsigned int j = 0; j < tempIndex.size(); j++)
						if(tempIndex[j] == index)
							okay = false;
				}
				tempIndex.push_back(index);
				Found = SearchForEmptySquare(index, SearchedSqrs, SearchDepth);
				if(Found){break;}
			}
			if(Found){break;}
			SearchDepth = SearchDepth+cleanDepth;
			if(SearchDepth >= MaxSearchDepth) //Failed; no new nodes avaliable
				return false;
		}
		//Target found
		CleanUpTarget = SearchedSqrs[SearchedSqrs.size()-1];
		//if(DynamicPath)
			//FindBestPath(CleanUpTarget,OQ);
		//else
			OldCleanupPath(CleanUpTarget,OQ);
	}

	//OQ is no longer empty, process the next action
	act nextAction = OQ[0];
	OQ.pop_front();
	DQ.push_back(nextAction);
	return true;
}
//-----------------------------------------------------------------------------------------------
//Search left/right of node for empty nodes
//-----------------------------------------------------------------------------------------------
bool MyBot::SearchForEmptySquare(int direction, SquareContainer & SearchedSqrs, int SearchDepth)
{
	//Virtually move hunter +depth in direction
	square vPos = Pos;
	if(direction == nY)
		vPos.y = vPos.y-SearchDepth;
	else if(direction == pY)
		vPos.y = vPos.y+SearchDepth;
	else if(direction == nX)
		vPos.x = vPos.x-SearchDepth;
	else if(direction == pX)
		vPos.x = vPos.x+SearchDepth;
	
	//add vpos to searched squares
	SearchedSqrs.push_back(vPos);

	//check if vpos is a target
	if(!NodeExists(vPos) && !SquareIsWall(vPos))
		return true;

	//check vpos side nodes for target
	square rvPos = vPos; square lvPos = vPos;
	for(int i = 0; i < SearchDepth; i++)
	{
		//get next squares left/right depth amount of node
		bool rvFound = false; bool lvFound = false;
		if((direction == nY)||(direction == pY)){rvPos.x++; lvPos.x--;}
		else{rvPos.y++; lvPos.y--;}

		//check if squares are in container
		for(unsigned int j = 0; j < SearchedSqrs.size(); j++)
		{
			if(SearchedSqrs[j] == rvPos)
				rvFound = true;
			else if(SearchedSqrs[j] == lvPos)
				lvFound = true;
			if(rvFound && lvFound)
				break;
		}

		//if vpos is a wall then all sides nodes will be walls too; add to container
		if(SquareIsWall(vPos))
		{
			if(!lvFound)
			{
				SearchedSqrs.push_back(lvPos);
				lvFound = true;
			}
			if(!rvFound)
			{
				SearchedSqrs.push_back(rvPos);
				rvFound = true;
			}
		}

		//if square hasn't been checked before
		if(!rvFound)
		{
			SearchedSqrs.push_back(rvPos);
			//check if a target square
			if(!NodeExists(rvPos))
			{
				if(!SquareIsWall(rvPos))
					return true;
			}
		}
		//if square hasn't been checked before
		if(!lvFound)
		{
			SearchedSqrs.push_back(lvPos);
			//check if a target square
			if(!NodeExists(lvPos))
			{
				if(!SquareIsWall(lvPos))
					return true;
			}
		}
	}
	//if searched all side nodes and nothing found, return
	return false;
}
//-----------------------------------------------------------------------------------------------
//Old Cleanup Paths
//-----------------------------------------------------------------------------------------------
bool MyBot::OldCleanupPath(square & Target, ActionQueue & Q)
{
	//Target found
	vec2D targetVec((Target.x-Pos.x),(Target.y-Pos.y)); //vector from target to position

	//Find x/y difference between current pos and target
	int yDif = int(sqrt(float(targetVec.y*targetVec.y)));
	int xDif = int(sqrt(float(targetVec.x*targetVec.x)));

	//Find whether target is below,above,left,right of hunter
	int newOrientation = Orientation;
	if((targetVec.x < 0)&&(targetVec.y < 0))
	{
		//bottom left to hunter; want down or left orientation
		if(Orientation == oUP){Q.push_back(act(TURNLEFT)); newOrientation = oLEFT;}
		else if(Orientation == oRIGHT){Q.push_back(act(TURNRIGHT)); newOrientation = oDOWN;}
		if(newOrientation == oLEFT)
		{
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNLEFT));
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
		}
		else if(newOrientation == oDOWN)
		{
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNRIGHT));
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
		}
	}
	else if((targetVec.x < 0)&&(targetVec.y > 0)) 
	{
		//top left to hunter; want up or left orientation
		if(Orientation == oDOWN){Q.push_back(act(TURNRIGHT)); newOrientation = oLEFT;}
		else if(Orientation == oRIGHT){Q.push_back(act(TURNLEFT)); newOrientation = oUP;}
		if(newOrientation == oUP)
		{
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNLEFT));
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
		}
		else if(newOrientation == oLEFT)
		{
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNRIGHT));
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
		}
	}
	else if((targetVec.x > 0)&&(targetVec.y < 0)) 
	{
		//bottom right to hunter; want down or right orientation
		if(Orientation == oUP){Q.push_back(act(TURNRIGHT)); newOrientation = oRIGHT;}
		else if(Orientation == oLEFT){Q.push_back(act(TURNLEFT)); newOrientation = oDOWN;}
		if(newOrientation == oDOWN)
		{
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNLEFT));
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
		}
		else if(newOrientation == oRIGHT)
		{
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNRIGHT));
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
		}
	}
	else if((targetVec.x > 0)&&(targetVec.y > 0))
	{
		//top right to hunter; want up or right orientation
		if(Orientation == oDOWN){Q.push_back(act(TURNLEFT)); newOrientation = oRIGHT;}
		else if(Orientation == oLEFT){Q.push_back(act(TURNRIGHT)); newOrientation = oUP;}
		if(newOrientation == oUP)
		{
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNRIGHT));
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
		}
		else if(newOrientation == oRIGHT)
		{
			for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
			Q.push_back(act(TURNLEFT));
			for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
		}
	}
	else if((targetVec.x == 0)&&(targetVec.y < 0))
	{
		//directly below hunter
		if(Orientation == oUP)
		{
			Q.push_back(act(TURNLEFT));
			Q.push_back(act(TURNLEFT));
		}
		else if(Orientation == oLEFT)
			Q.push_back(act(TURNLEFT));
		else if(Orientation == oRIGHT)
			Q.push_back(act(TURNRIGHT));
		for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
	}
	else if((targetVec.x == 0)&&(targetVec.y > 0))
	{
		//directly above hunter
		if(Orientation == oDOWN)
		{
			Q.push_back(act(TURNLEFT));
			Q.push_back(act(TURNLEFT));
		}
		else if(Orientation == oLEFT)
			Q.push_back(act(TURNRIGHT));
		else if(Orientation == oRIGHT)
			Q.push_back(act(TURNLEFT));
		for(int k = 0; k < yDif; k++){Q.push_back(act(STEP));}
	}
	else if((targetVec.x < 0)&&(targetVec.y == 0))
	{
		//directly to left of hunter
		if(Orientation == oRIGHT)
		{
			Q.push_back(act(TURNLEFT));
			Q.push_back(act(TURNLEFT));
		}
		else if(Orientation == oDOWN)
			Q.push_back(act(TURNRIGHT));
		else if(Orientation == oUP)
			Q.push_back(act(TURNLEFT));
		for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
	}
	else if((targetVec.x > 0)&&(targetVec.y == 0))
	{
		//directly to right of hunter
		if(Orientation == oLEFT)
		{
			Q.push_back(act(TURNLEFT));
			Q.push_back(act(TURNLEFT));
		}
		else if(Orientation == oUP)
			Q.push_back(act(TURNRIGHT));
		else if(Orientation == oDOWN)
			Q.push_back(act(TURNLEFT));
		for(int k = 0; k < xDif; k++){Q.push_back(act(STEP));}
	}
	else
		return false; //FAILED; 0,0 found
	return true;
}