#include "MyBot.h"

//COMMANDS
//z = FORWARD, RIGHT, BACK, LEFT [p.view[z] == y]
//y = EMPTY, FOG, WALL, HUNTER, SHROOM, MAGICSHROOM 
//x = QUIT, STEP, TURNLEFT, TURNRIGHT, LOOK, EAT [a = x]

//-----------------------------------------------------------------------------------------------
//OUTERSPIRAL
//-----------------------------------------------------------------------------------------------
enum {sSTART = 0, sSPIRAL, sWALL, sWALLFIX, sSPIRALFIX};
void MyBot::OuterSpiral(const PERCEPT &p)
{
	static int State = sSTART;
	static int stepsNeeded = 7;
	static int stepsTaken = 0;
	static int corner = 1;
	static bool turnright = true;

	//Move until reaching a square that's not in no-go zone, if hit a wall before then turn and contiune
	//Pattern: 2 5 7 10 10 13 13....
	switch(State)
	{
	case sSTART:
		{
			static int counter = 0;
			static int initialsteps = 2;
			if((p.view[FORWARD] == WALL)&&(counter < initialsteps))
			{
				//TO FIX
				//currently unsure of fix for spiral, default to zigzag if hit wall straight away
				//ChangePath(ZIGZAG);
			}
			if(counter < initialsteps)
			{
				DQ.push_back(act(STEP));//step for two/five paces
				counter++;
			} 
			else
			{
				DQ.push_back(act(TURNRIGHT));
				counter = 0;
				initialsteps = 5;
				static int counter2 = 0;
				counter2++;
				if(counter2 >= 2)
					State = sSPIRAL;
			}
			break;
		}
	case sSPIRAL:
		{
			if((p.view[FORWARD] == WALL)&&(stepsTaken < stepsNeeded))
			{
				State = sWALL;
				OuterSpiral(p);
			}
			if(stepsTaken < stepsNeeded)
			{
				DQ.push_back(act(STEP));
				stepsTaken++;
			}
			else
			{				
				if(turnright){DQ.push_back(act(TURNRIGHT));}
				else {DQ.push_back(act(TURNLEFT));}

				//add three every second corner
				stepsTaken = 0;
				corner++;
				if(corner >=2)
				{
					stepsNeeded = stepsNeeded+3;
					corner = 0;
				}
			}
			break;
		}
	case sWALL: //If a wall is hit which breaks the spiral
		{
			if((p.view[FORWARD])&&(p.view[FORWARD]||p.view[FORWARD])) 
			{
				//IF STUCK IN CORNER [TO FIX]
				//ChangePath(ZIGZAG);
			}
			if(turnright)
			{
				DQ.push_back(act(TURNLEFT));
				turnright = false;
			}
			else 
			{
				DQ.push_back(act(TURNRIGHT));
				turnright = true;
			}
			State = sWALLFIX;
			break;
		}
	case sWALLFIX: //Move two paces away from sprial and turn
		{
			static int counter = 0;
			if((p.view[FORWARD] == WALL)&&(counter < 2))
			{
				counter = 0;
				State = sWALL;
				OuterSpiral(p);
			}
			if(counter < 2)
			{
				DQ.push_back(act(STEP));//step for two paces
				counter++;
			} 
			else
			{
				if(turnright){DQ.push_back(act(TURNRIGHT));}
				else {DQ.push_back(act(TURNLEFT));}	
				State = sSPIRALFIX;
				counter = 0;
			}
			break;
		}
	case sSPIRALFIX: //Move around spiral in opposite direction
		{
			static int counter = 0;
			if((p.view[FORWARD] == WALL)&&(counter < (stepsTaken+2)))
			{
				counter = 0;
				State = sWALL;
				OuterSpiral(p);
			}
			if(counter < (stepsTaken+2))
			{
				DQ.push_back(act(STEP));  //go up amount actually done+3
				counter++;
			}
			else
			{
				if(turnright){DQ.push_back(act(TURNRIGHT));}
				else {DQ.push_back(act(TURNLEFT));}	
				stepsTaken = 0;
				stepsNeeded = stepsNeeded + 6;
				corner = 1;
				State = sSPIRAL;
				counter = 0;
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------
//ZIGZAG
//-----------------------------------------------------------------------------------------------
enum {zZig = 0, zZag, zReturn};
void MyBot::ZigZag(const PERCEPT &p)
{
	static int State = zZig;
	static int LastState;
	switch(State)
	{
		case zZig: //--------------------------------->
		{
			static int ZigOrientation = Orientation;			
			bool WallForward = false;
			node* forwardNode = FindNode(FORWARD,1);
			if((p.view[FORWARD] == WALL)||(forwardNode->virtualWall))
				WallForward = true;

			if((p.view[LEFT] == WALL)&&(WallForward)) //if reached a wall and wall in way of turn
			{
				DQ.push_back(act(TURNRIGHT)); //turn right and change state
				LastState = State;
				State = zReturn;
			}
			else if((p.view[RIGHT] == WALL)&&(Orientation != ZigOrientation)) //if turned, move up three paces if possible and turn/set state
			{
				static int counter = 0;
				if(counter > 2) //if moved three paces or wall hit
				{
					LastState = State;
					counter = 0;
					State = zZag;
					DQ.push_back(act(TURNLEFT)); //turn/set state
				}
				else if(WallForward)
				{
					node* stateTest = FindNode(LEFT,2);
					if(stateTest == NULL)
					{
						//if square hasn't been visited
						LastState = State;
						counter = 0;
						State = zZag;
						DQ.push_back(act(TURNLEFT)); //turn/set state
					}
					else
					{
						DQ.push_back(act(TURNLEFT)); //turn right and change state
						DQ.push_back(act(TURNLEFT));
						LastState = State;
						State = zReturn;
					}
				}
				else
				{
					DQ.push_back(act(STEP));
					counter++;
				}
			}
			else if(WallForward) //if reached a wall, turn 
				DQ.push_back(act(TURNLEFT));
			else
				DQ.push_back(act(STEP)); //keep moving if no wall
			break;
		}
		case zZag: // <---------------------------------
		{
			static int ZagOrientation = Orientation;		
			bool WallForward = false;
			node* forwardNode = FindNode(FORWARD,1);
			if((p.view[FORWARD] == WALL)||(forwardNode->virtualWall))
				WallForward = true;

			if((p.view[RIGHT] == WALL)&&(WallForward)) //if reached a wall and wall in way of turn
			{
				DQ.push_back(act(TURNLEFT)); //turn left and change state
				LastState = State;
				State = zReturn;
			}
			else if((p.view[LEFT] == WALL)&&(Orientation != ZagOrientation)) //if turned, move up three paces if possible and turn/set state
			{
				static int counter = 0;
				if(counter > 2) //if moved three paces or wall hit
				{
					LastState = State;
					State = zZig;
					DQ.push_back(act(TURNRIGHT)); //turn/set state
					counter = 0;
				}
				else if(WallForward)
				{
					node* stateTest = FindNode(RIGHT,2);
					if(stateTest == NULL)
					{
						//if square hasn't been visited
						LastState = State;
						counter = 0;
						State = zZag;
						DQ.push_back(act(TURNLEFT)); //turn/set state
					}
					else
					{
						DQ.push_back(act(TURNRIGHT)); //turn right and change state
						DQ.push_back(act(TURNRIGHT));
						LastState = State;
						State = zReturn;
					}
				}
				else
				{
					DQ.push_back(act(STEP));
					counter++;
				}
			}
			else if(WallForward) //if reached a wall, turn 
				DQ.push_back(act(TURNRIGHT));
			else
				DQ.push_back(act(STEP)); //keep moving if no wall
			break;
		}
		case zReturn:  ///////////////////////////////////////////////////////////////////////////////////////////
		{
			//Move forward until hitting a wall
			if(p.view[FORWARD] == WALL)
			{
				if(LastState == zZig)
				{
					LastState = State;
					State = zZag;
					DQ.push_back(act(TURNRIGHT));
				}
				else if(LastState == zZag)
				{
					LastState = State;
					State = zZig;
					DQ.push_back(act(TURNLEFT));
				}
			}
			else
			{
				DQ.push_back(act(STEP));
				Map[K(Pos.x,Pos.y)].virtualWall = true; //create a virtual wall
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------
//CLEANUP
//-----------------------------------------------------------------------------------------------
void MyBot::CleanUp(const PERCEPT &p)
{
	if(!MoveTowardsNearestBlankNode())
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
bool MyBot::MoveTowardsNearestBlankNode()
{
	//returns true working, returns false if no nodes can be found
	//Decides directions to get to the closest blank node from position and queues
	int OQsize = OQ.size();
	
	//Check path created didn't kill hunter and redo it if it did
	//if(WallSafetyNet())
	//	OQ.clear();

	while(OQ.empty())
	{
		SquareContainer SearchedSqrs;
		int SearchDepth = 1;
		int MaxSearchDepth = max(MapColumns,MapRows);
		bool Found = false;
		square vPos;

		while(!Found)
		{
			//for all four directions
			for(int i = 0; i < 4; i++)
			{
				Found = SearchForEmptySquare(i, SearchedSqrs, SearchDepth);
				if(Found){break;}
			}
			if(Found){break;}
			SearchDepth++;
			if(SearchDepth >= MaxSearchDepth) //Failed; no new nodes avaliable
				return false;
		}
		//Target found
		square Target = SearchedSqrs[SearchedSqrs.size()-1];
		vec2D targetVec((Target.x-Pos.x),(Target.y-Pos.y)); //vector from target to position

		//TESTING
		Pos.x = Pos.x;
		Pos.y = Pos.y;
		if((Target.x == Target.y) && (Target.x == 8))
			Target.x = Target.y;


		//Find x/y difference between current pos and target
		int yDif = int(sqrt(float(targetVec.y*targetVec.y)));
		int xDif = int(sqrt(float(targetVec.x*targetVec.x)));

		//Find whether target is below,above,left,right of hunter
		int newOrientation = Orientation;
		if((targetVec.x < 0)&&(targetVec.y < 0))
		{
			//bottom left to hunter; want down or left orientation
			if(Orientation == oUP){OQ.push_back(act(TURNLEFT)); newOrientation = oLEFT;}
			else if(Orientation == oRIGHT){OQ.push_back(act(TURNRIGHT)); newOrientation = oDOWN;}
			if(newOrientation == oLEFT)
			{
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNLEFT));
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
			}
			else if(newOrientation == oDOWN)
			{
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNRIGHT));
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
			}
		}
		else if((targetVec.x < 0)&&(targetVec.y > 0)) 
		{
			//top left to hunter; want up or left orientation
			if(Orientation == oDOWN){OQ.push_back(act(TURNRIGHT)); newOrientation = oLEFT;}
			else if(Orientation == oRIGHT){OQ.push_back(act(TURNLEFT)); newOrientation = oUP;}
			if(newOrientation == oUP)
			{
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNRIGHT));
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
			}
			else if(newOrientation == oLEFT)
			{
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNLEFT));
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
			}
		}
		else if((targetVec.x > 0)&&(targetVec.y < 0)) 
		{
			//bottom right to hunter; want down or right orientation
			if(Orientation == oUP){OQ.push_back(act(TURNRIGHT)); newOrientation = oRIGHT;}
			else if(Orientation == oLEFT){OQ.push_back(act(TURNLEFT)); newOrientation = oDOWN;}
			if(newOrientation == oDOWN)
			{
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNLEFT));
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
			}
			else if(newOrientation == oRIGHT)
			{
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNRIGHT));
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
			}
		}
		else if((targetVec.x > 0)&&(targetVec.y > 0))
		{
			//top right to hunter; want up or right orientation
			if(Orientation == oDOWN){OQ.push_back(act(TURNLEFT)); newOrientation = oRIGHT;}
			else if(Orientation == oLEFT){OQ.push_back(act(TURNRIGHT)); newOrientation = oUP;}
			if(newOrientation == oUP)
			{
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNRIGHT));
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
			}
			else if(newOrientation == oRIGHT)
			{
				for(int k = 0; k < xDif; k++){OQ.push_back(act(STEP));}
				OQ.push_back(act(TURNLEFT));
				for(int k = 0; k < yDif; k++){OQ.push_back(act(STEP));}
			}
		}
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
		vPos.y = vPos.x-SearchDepth;
	else if(direction == pX)
		vPos.y = vPos.x+SearchDepth;

	//Make sure vpos isn't a wall; vPos will definately have a node
	if(Map[K(vPos)].type == WALL) 
	{
		SearchedSqrs.push_back(vPos);
		return false;
	}
	else
	{
		square rvPos = vPos; square lvPos = vPos;
		for(int i = 0; i < SearchDepth; i++)
		{
			//get next squares left/right depth amount of node
			if((direction == nY)||(direction == pY)){rvPos.x++; lvPos.x--;}
			else{rvPos.x++; lvPos.x--;}

			//check if squares are in container
			bool rvFound = false; bool lvFound = false;
			for(unsigned int j = 0; j < SearchedSqrs.size(); j++)
			{
				if(SearchedSqrs[j] == rvPos)
					rvFound = true;
				else if(SearchedSqrs[j] == lvPos)
					lvFound = true;
			}

			//if square hasn't been checked before
			if(!rvFound)
			{
				//if square isn't a wall, mark as found
				if(SquareIsWall(rvPos) != 1)
					SearchedSqrs.push_back(rvPos);
				else
					return false;
				
				//check if a target square
				if(!NodeExists(rvPos))
					return true;
			}
			//if square hasn't been checked before
			if(!lvFound)
			{
				//if square isn't a wall, mark as found
				if(SquareIsWall(lvPos) != 1)
					SearchedSqrs.push_back(lvPos);
				else
					return false;
				
				//check if a target square
				if(!NodeExists(lvPos))
					return true;
			}
		}
	}
	//if searched all side nodes and nothing found, return
	return false;
}