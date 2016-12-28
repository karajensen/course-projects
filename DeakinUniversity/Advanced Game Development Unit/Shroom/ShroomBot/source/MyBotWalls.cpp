#include "MyBot.h"

//-----------------------------------------------------------------------------------------------
//Check if target obscured by wall
//-----------------------------------------------------------------------------------------------
bool MyBot::SafetyNet()
{
	//Safety net for walls: true to use
	//if the character hasn't moved and last action was a step, assume stuck and fix
	square coord = FindCoordinates(FORWARD,1);
	if((NextAction.action == STEP) && (SquareIsWall(coord)))
		return true;
	else if((Pos == LastPos)&&(LastAction.action == STEP))
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------
//Check if square is a wall or beyond
//-----------------------------------------------------------------------------------------------
bool MyBot::SquareIsWall(int x, int y)
{
	if(MapWalls.FoundAllWalls) //If all walls found, check all
	{
		if((x > MapWalls.topleft.x)&&
		   (x < MapWalls.bottomright.x)&&
		   (y > MapWalls.bottomright.y)&&
		   (y < MapWalls.topleft.y))
		   return false;
		else
			return true;
	}
	else //If not all walls found, check individually
	{
		if(MapWalls.DownWallReached)
		{
			if(y <= MapWalls.bottomright.y) 
				return true;
		}
		if(MapWalls.LeftWallReached)
		{
			if(x <= MapWalls.topleft.x) 
				return true;
		}
		if(MapWalls.RightWallReached)
		{
			if(x >= MapWalls.bottomright.x)
				return true;
		}
		if(MapWalls.UpWallReached)
		{
			if(y >= MapWalls.topleft.y)
				return true;
		}
		return false;
	}
}
bool MyBot::SquareIsWall(square & coordinates)
{
	return SquareIsWall(coordinates.x,coordinates.y);
}
bool MyBot::SquareIsWall(node & n)
{
	return SquareIsWall(n.coord.x,n.coord.y);
}
//-----------------------------------------------------------------------------------------------
//Check and update walls
//-----------------------------------------------------------------------------------------------
void MyBot::CheckWalls(const PERCEPT &p)
{
	//ROWS IS SAME AS COLUMNS: NEED MIN 2 WALLS
	if((MapColumns == MapRows)&&(!MapWalls.FoundAllWalls))
	{
		if(!MapWalls.UpWallReached)
		{
			if(	((p.view[FORWARD] == WALL)&&(Orientation == oUP))||
				((p.view[BACK] == WALL)&&(Orientation == oDOWN))||
				((p.view[LEFT] == WALL)&&(Orientation == oRIGHT))||
				((p.view[RIGHT] == WALL)&&(Orientation == oLEFT)))
			{
				//TOP WALL
				MapWalls.topleft.y = Pos.y+1;
				MapWalls.bottomright.y = Pos.y-MapRows;
				MapWalls.UpWallReached = true;
				MapWalls.DownWallReached = true;
			}
		}
		if(!MapWalls.DownWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oDOWN))||
				((p.view[BACK] == WALL)&&(Orientation == oUP))||
				((p.view[RIGHT] == WALL)&&(Orientation == oRIGHT))||
				((p.view[LEFT] == WALL)&&(Orientation == oLEFT)))
			{
				//BOTTOM WALL
				MapWalls.topleft.y = Pos.y+MapRows;
				MapWalls.bottomright.y = Pos.y-1;
				MapWalls.UpWallReached = true;
				MapWalls.DownWallReached = true;
			}
		}
		if(!MapWalls.LeftWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oLEFT))||
				((p.view[LEFT] == WALL)&&(Orientation == oUP))||
				((p.view[RIGHT] == WALL)&&(Orientation == oDOWN))||
				((p.view[BACK] == WALL)&&(Orientation == oRIGHT)))
			{
				//LEFT WALL
				MapWalls.topleft.x = Pos.x-1;
				MapWalls.bottomright.x = Pos.x+MapColumns;
				MapWalls.LeftWallReached = true;
				MapWalls.RightWallReached = true;
			}
		}
		if(!MapWalls.RightWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oRIGHT))||
				((p.view[BACK] == WALL)&&(Orientation == oLEFT))||
				((p.view[LEFT] == WALL)&&(Orientation == oDOWN))||
				((p.view[RIGHT] == WALL)&&(Orientation == oUP)))
			{
				//RIGHT WALL
				MapWalls.topleft.x = Pos.x-MapColumns;
				MapWalls.bottomright.x = Pos.x+1;
				MapWalls.LeftWallReached = true;
				MapWalls.RightWallReached = true;
			}
		}
		if(MapWalls.RightWallReached && MapWalls.LeftWallReached &&
			MapWalls.DownWallReached && MapWalls.UpWallReached)
			MapWalls.FoundAllWalls = true;

	}//COLUMNS AND ROWS NOT THE SAME: NEED MIN 3 WALLS
	else if((MapColumns != MapRows)&&(!MapWalls.FoundAllWalls)) 
	{	
		if(!MapWalls.UpWallReached)
		{
			if(	((p.view[FORWARD] == WALL)&&(Orientation == oUP))||
				((p.view[BACK] == WALL)&&(Orientation == oDOWN))||
				((p.view[LEFT] == WALL)&&(Orientation == oRIGHT))||
				((p.view[RIGHT] == WALL)&&(Orientation == oLEFT)))
			{
				//TOP WALL
				MapWalls.topleft.y = Pos.y+1;
				MapWalls.UpWallReached = true;
			}
		}
		if(!MapWalls.DownWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oDOWN))||
				((p.view[BACK] == WALL)&&(Orientation == oUP))||
				((p.view[RIGHT] == WALL)&&(Orientation == oRIGHT))||
				((p.view[LEFT] == WALL)&&(Orientation == oLEFT)))
			{
				//BOTTOM WALL
				MapWalls.bottomright.y = Pos.y-1;
				MapWalls.DownWallReached = true;
			}
		}
		if(!MapWalls.LeftWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oLEFT))||
				((p.view[LEFT] == WALL)&&(Orientation == oUP))||
				((p.view[RIGHT] == WALL)&&(Orientation == oDOWN))||
				((p.view[BACK] == WALL)&&(Orientation == oRIGHT)))
			{
				//LEFT WALL
				MapWalls.topleft.x = Pos.x-1;
				MapWalls.LeftWallReached = true;
			}
		}
		if(!MapWalls.RightWallReached)
		{
			if(((p.view[FORWARD] == WALL)&&(Orientation == oRIGHT))||
				((p.view[BACK] == WALL)&&(Orientation == oLEFT))||
				((p.view[LEFT] == WALL)&&(Orientation == oDOWN))||
				((p.view[RIGHT] == WALL)&&(Orientation == oUP)))
			{
				//RIGHT WALL
				MapWalls.bottomright.x = Pos.x+1;
				MapWalls.RightWallReached = true;
			}
		}
		//After 3 walls have been found, calculate the forth
		if(MapWalls.RightWallReached && MapWalls.LeftWallReached)
		{
			int distance = (MapWalls.bottomright.x - MapWalls.topleft.x)+1;
			if(distance < 0){distance = distance*-1;}

			if(MapWalls.DownWallReached) //CHECKED
			{
				if(distance == WorldColumns) //distance is columns, use rows
					MapWalls.topleft.y = MapWalls.bottomright.y + MapRows +1;
				else if(distance == WorldRows) //distance is rows, use columns
					MapWalls.topleft.y = MapWalls.bottomright.y + MapColumns +1;
				MapWalls.UpWallReached = true;
				MapWalls.FoundAllWalls = true;
			}
			else if(MapWalls.UpWallReached) //CHECKED
			{
				if(distance == WorldColumns) //distance is columns, use rows
					MapWalls.bottomright.y = MapWalls.topleft.y - MapRows - 1;
				else if(distance == WorldRows) //distance is rows, use columns
					MapWalls.bottomright.y = MapWalls.topleft.y - MapColumns - 1;
				MapWalls.DownWallReached = true;
				MapWalls.FoundAllWalls = true;
			}
		}
		else if(MapWalls.UpWallReached && MapWalls.DownWallReached)
		{
			int distance = (MapWalls.bottomright.y - MapWalls.topleft.y)-1;
			if(distance < 0){distance = distance*-1;}

			if(MapWalls.RightWallReached) //CHECKED
			{
				if(distance == WorldColumns) //distance is columns, use rows
					MapWalls.topleft.x = MapWalls.bottomright.x - MapRows - 1;
				else if(distance == WorldRows) //distance is rows, use columns
					MapWalls.topleft.x = MapWalls.bottomright.x - MapColumns - 1;
				MapWalls.LeftWallReached = true;
				MapWalls.FoundAllWalls = true;
			}
			else if(MapWalls.LeftWallReached) //CHECKED
			{
				if(distance == WorldColumns) //distance is columns, use rows
					MapWalls.bottomright.x = MapWalls.topleft.x + MapRows + 1;
				else if(distance == WorldRows) //distance is rows, use columns
					MapWalls.bottomright.x = MapWalls.topleft.x + MapColumns + 1;
				MapWalls.RightWallReached = true;
				MapWalls.FoundAllWalls = true;
			}
		}
	}
}