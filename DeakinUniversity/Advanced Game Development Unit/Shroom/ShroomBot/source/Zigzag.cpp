#include "MyBot.h"
//-----------------------------------------------------------------------------------------------
//ZIGZAG
//-----------------------------------------------------------------------------------------------
enum {zZig = 0, zZag, zReturn};
void MyBot::ZigZag(const PERCEPT &p, bool reset)
{
	static int State;
	static int LastState;
	static int ZigOrientation;
	static int Zigcounter;
	static int ZagOrientation;
	static int Zagcounter;
	static bool MovingUp;
	static bool returned;

	if(reset)
	{
		returned = false;
		State = zZig;
		ZigOrientation = Orientation;
		Zigcounter = 0;
		ZagOrientation = Orientation;
		Zagcounter = 0;
		MovingUp = false;
		return;
	}

	//check for walls
	bool WallForward = false;
	node* forwardNode = FindNode(FORWARD,3);
	square forwardCoord = FindCoordinates(FORWARD,2);
	if(p.view[FORWARD] == WALL)
		WallForward = true;
	else if(SquareIsWall(forwardCoord.x,forwardCoord.y))
		WallForward = true;
	else if(forwardNode != NULL)
	{
		if(forwardNode->virtualWall)
			WallForward = true;
	}

	square forwardx1 = FindCoordinates(FORWARD,1);
	if((forwardx1.x < -2) && returned && (State != zReturn))
	{
		PathChosen = CLEANUP;
		CleanUp(p,true);
		return;
	}

	switch(State)
	{
		case zZig: //--------------------------------->
		{
			//if reached a wall and wall in way of turn
			if((p.view[LEFT] == WALL)&&(WallForward))
			{
				DQ.push_back(act(TURNRIGHT)); //turn right and change state
				LastState = State;
				State = zReturn;
			}
			else if(MovingUp) //if turned
			{
				if(Zigcounter > 2) //if moved three paces
				{
					LastState = State;
					Zigcounter = 0;
					State = zZag;
					DQ.push_back(act(TURNLEFT)); 
					MovingUp = false;
				}
				else if(WallForward) //if hit a wall before 3 paces
				{
					node* stateTest = FindNode(LEFT,2);
					if(stateTest == NULL)
					{
						//if square hasn't been visited
						LastState = State;
						Zigcounter = 0;
						State = zZag;
						DQ.push_back(act(TURNLEFT));
					}
					else
					{
						//return
						DQ.push_back(act(TURNLEFT)); 
						DQ.push_back(act(TURNLEFT));
						LastState = State;
						State = zReturn;
						Zigcounter = 0;
					}
					MovingUp = false;
				}
				else
				{
					//move up
					DQ.push_back(act(STEP));
					Zigcounter++;
				}
			}
			else if(WallForward) //if reached a wall, turn 
			{
				DQ.push_back(act(TURNLEFT));
				MovingUp = true;
			}
			else
				DQ.push_back(act(STEP)); //keep moving if no wall
			break;
		}
		case zZag: // <---------------------------------
		{	

			 //if reached a wall and wall in way of turn
			if((p.view[RIGHT] == WALL)&&(WallForward))
			{
				DQ.push_back(act(TURNLEFT));
				LastState = State;
				State = zReturn;
			}
			else if(MovingUp) //if turned
			{
				if(Zagcounter > 2) //if moved three paces already
				{
					LastState = State;
					State = zZig;
					DQ.push_back(act(TURNRIGHT));
					Zagcounter = 0;
					MovingUp = false;
				}
				else if(WallForward) //if wall hit before reaching 3 paces
				{
					node* stateTest = FindNode(RIGHT,2);
					if(stateTest == NULL)
					{
						//if square hasn't been visited
						LastState = State;
						Zagcounter = 0;
						State = zZag;
						DQ.push_back(act(TURNLEFT)); 
					}
					else
					{
						//return
						Zagcounter = 0;
						DQ.push_back(act(TURNRIGHT)); 
						DQ.push_back(act(TURNRIGHT));
						LastState = State;
						State = zReturn;
					}
					MovingUp = false;
				}
				else //move up
				{
					DQ.push_back(act(STEP));
					Zagcounter++;
				}
			}
			else if(WallForward) //if reached a wall, turn 
			{
				DQ.push_back(act(TURNRIGHT));
				MovingUp = true;
			}
			else
				DQ.push_back(act(STEP)); //keep moving if no wall
			break;
		}
		case zReturn:  ///////////////////////////////////////////////////////////////////////////////////////////
		{
			//Move forward until hitting a wall
			returned = true;
			bool WallFound = false;
			square forwardCoord = FindCoordinates(FORWARD,2);
			if(p.view[FORWARD] == WALL)
				WallFound = true;
			else if(SquareIsWall(forwardCoord.x,forwardCoord.y))
				WallFound = true;

			if(WallFound)
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
