#include "MyBot.h"
//-----------------------------------------------------------------------------------------------
//INNER SPIRAL
//-----------------------------------------------------------------------------------------------
enum {iSTART=0,iSPIRAL};
void MyBot::InnerSpiral(const PERCEPT &p, bool reset)
{
	static int State;

	if(reset)
	{
		State = iSTART;
		return;
	}

	switch(State)
	{
	case iSTART:
		{
			//if a wall behind, rotate left
			if(p.view[BACK] == WALL)
			{
				DQ.push_back(act(TURNLEFT));

			}//if wall to right
			else if((p.view[RIGHT] == WALL)&&(p.view[FORWARD] != WALL))
			{
				DQ.push_back(act(TURNRIGHT));

			}//if a wall to left
			else if((p.view[LEFT] == WALL)&&(p.view[FORWARD] != WALL))
			{
				//create nodes for max(MapColumns,MapRows) directly left of hunter
				int NoOfNodes = max(MapColumns,MapRows);
				int x = Pos.x; int y = Pos.y;
				for(int i = 0; i < NoOfNodes; i++)
				{
					y--;
					CreateNewNode(EMPTY,x,y); //set new nodes as virtual walls
					Map[K(x,y)].virtualWall = true;
				}
				//Set to sprial
				State = iSPIRAL;
				InnerSpiral(p,false);
			}//Find a wall
			else if(p.view[FORWARD] == WALL)
			{
				//create nodes for max(MapColumns,MapRows) directly left of hunter
				int NoOfNodes = max(MapColumns,MapRows);
				int x = Pos.x; int y = Pos.y;
				for(int i = 0; i < NoOfNodes; i++)
				{
					x--;
					CreateNewNode(EMPTY,x,y); //set new nodes as virtual walls
					Map[K(x,y)].virtualWall = true;
				}
				//Turn right and set state to spiral
				DQ.push_back(act(TURNRIGHT));
				State = iSPIRAL;
			}
			else
				DQ.push_back(act(STEP));
			break;
		}
	case iSPIRAL:
		{
			//leave path of Virtual walls
			Map[K(Pos.x,Pos.y)].virtualWall = true;
	
			//get surrounding nodes
			bool vWallForward = false;
			bool vWallRight = false;
			node * rightNode = FindNode(RIGHT,1);
			node * forwardNode = FindNode(FORWARD,3);
			square forwardCoord = FindCoordinates(FORWARD,2);
			
			if(SquareIsWall(forwardCoord.x,forwardCoord.y))
				vWallForward = true;
	
			if(forwardNode != NULL)
			{
				if(forwardNode->virtualWall)
				{
					node * test = FindNode(FORWARD,3);
					forwardNode->coord.x = forwardNode->coord.x;
					forwardNode->coord.y = forwardNode->coord.y;
					vWallForward = true;
				}
			}
			if(rightNode != NULL)
			{
				if(rightNode->virtualWall)
					vWallRight = true;
			}

			if(Pos.y == 0)
				int x = 0;

			//if virtual wall all around, call cleanup function
			if(vWallForward && vWallRight )
			{
				PathChosen = CLEANUP;
				GenerateActions(p);
				break;
			}

			//if wall or virtual wall is forward, turn right
			if((p.view[FORWARD] == WALL) || (vWallForward))
			{
				Pos.x = Pos.x;
				DQ.push_back(act(TURNRIGHT));
			}
			else
				DQ.push_back(act(STEP));

			break;
		}
	}
}