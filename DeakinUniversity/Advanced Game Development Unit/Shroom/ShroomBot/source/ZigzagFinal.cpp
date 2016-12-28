#include "MyBot.h"
//-----------------------------------------------------------------------------------------------
//ZIGZAG
//-----------------------------------------------------------------------------------------------
const bool LeaveEdges = false;

enum{zZIGZAG = 0, zRETURN, zSTART};
static bool turnright;
void ToggleZigZagFinalTurn()
{
	if(turnright)
		turnright = false;
	else
		turnright = true;
}
void MyBot::ZigZagFinalTurn()
{
	if(turnright)
		DQ.push_back(act(TURNRIGHT));
	else
		DQ.push_back(act(TURNLEFT));
}

void MyBot::ZigZagFinal(const PERCEPT &p, bool reset)
{
	static bool movingup;
	static int State;
	static int movingupCounter;
	static bool returned;
	static bool returning;
	static bool movingacross;
	static int zigzagCounter;
	static bool startingFromLeftWall;
	static bool movingFromLeftWall;
	static bool firstZig;
	static bool secondZig;
	static bool reversedZig;
	if(reset)
	{
		secondZig = false;
		reversedZig = false;
		firstZig = true;
		zigzagCounter = 0;
		movingacross = true;
		returned = false;
		returning = false;
		movingupCounter = 0;
		movingup = false;
		turnright = true;
		startingFromLeftWall = false;
		movingFromLeftWall = false;
		State = zSTART;
		return;
	}

	//IF REACHED STARTING POINT
	square forwardx1 = FindCoordinates(FORWARD,1);
	if((forwardx1.x > 5) && returned && !returning && !startingFromLeftWall)
	{
		PathChosen = CLEANUP;
		CleanUp(p,true);
		return;
	}

	switch(State)
	{
	case zZIGZAG:
		{
			square forwardx2 = FindCoordinates(FORWARD,2);
			square forwardx1 = FindCoordinates(FORWARD,1);
			square forwardx3 = FindCoordinates(FORWARD,3);
			square forwardx5 = FindCoordinates(FORWARD,5);
			square rightx1 = FindCoordinates(RIGHT,1);
			square rightx2 = FindCoordinates(RIGHT,2);
			square rightx3 = FindCoordinates(RIGHT,3);
			square rightx4 = FindCoordinates(RIGHT,4);
			square rightx5 = FindCoordinates(RIGHT,5);
			square leftx1 = FindCoordinates(LEFT,1);
			square leftx2 = FindCoordinates(LEFT,2);
			square leftx3 = FindCoordinates(LEFT,3);
			square leftx4 = FindCoordinates(LEFT,4);
			square leftx5 = FindCoordinates(LEFT,5);

			//checks
			bool RightWall = false;
			bool LeftWall = false;
			if(SquareIsWall(leftx1)){LeftWall = true;}
			if(SquareIsWall(leftx2)){LeftWall = true;}
			if(SquareIsWall(leftx3)){LeftWall = true;}
			if(SquareIsWall(rightx1)){RightWall = true;}
			if(SquareIsWall(rightx2)){RightWall = true;}
			if(SquareIsWall(rightx3)){RightWall = true;}


			//////////////////////////////////////////////////////////////MOVING ACROSS
			if(movingacross)
			{
				if(!returned) //NOT RETURNED
				{
					//MOVING ACROSS, REACH TOP CORNER
					if((RightWall || LeftWall) && 
					   (SquareIsWall(forwardx2)||SquareIsWall(forwardx3)||SquareIsWall(forwardx1)))
					{
						if(startingFromLeftWall)
						{
							movingup = true;
							startingFromLeftWall = false;
							movingFromLeftWall = true;
						}
						else
						{
							ToggleZigZagFinalTurn();
							ZigZagFinalTurn();
							State = zRETURN;
						}
					
					}//MOVING ACROSS, REACH TOP CORNER, LEAVING EDGES
					else if((RightWall || LeftWall) && (SquareIsWall(forwardx5) && LeaveEdges))
					{
						if(startingFromLeftWall)
						{
							movingup = true;
							startingFromLeftWall = false;
							movingFromLeftWall = true;
						}
						else
						{
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							ToggleZigZagFinalTurn();
							ZigZagFinalTurn();
							State = zRETURN;
						}

					}//MOVING ACROSS, WALL IN FRONT
					else if(SquareIsWall(forwardx2)||SquareIsWall(forwardx1))
					{
						movingup = true;

					}//MOVING ACROSS, WALL IN FRONT,LEAVING EDGES
					else if(SquareIsWall(forwardx5) && LeaveEdges)
					{
						movingup = true;

					}
				}
				else //RETURNED
				{
					if(SquareIsWall(forwardx5))
					{
						if(NodeExists(forwardx2))
						{
							movingup = true;
						}
						else if(NodeExists(forwardx3))
						{
							DQ.push_back(act(STEP));
							movingup = true;
						}
						else
						{
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							movingup = true;
						}
					}
				}
			}

			//////////////////////////////////////////////////////////////MOVING UP
			if(movingup)
			{

				if(firstZig && movingupCounter >= 3)
				{
					secondZig = true;
					firstZig = false;
					movingupCounter = 0;
				}
				
				if(secondZig && movingupCounter >= 3)
					secondZig = false;

				if(movingacross) //if moving up initiated, turn up
				{
					movingacross = false;
					ZigZagFinalTurn();
				}
				else if(movingupCounter >= 3) //if reached end of moving up
				{
					movingacross = true;
					ZigZagFinalTurn();
					ToggleZigZagFinalTurn();
					movingup = false;
					movingupCounter = 0;
					zigzagCounter = 0;

				} //x2 or no spaces left of row when reaching top
				else if(SquareIsWall(forwardx2)||SquareIsWall(forwardx1))
				{
					if(((!SquareIsWall(rightx4))&&(SquareIsWall(rightx5)))||
					   ((!SquareIsWall(leftx4))&&(SquareIsWall(leftx5))) && LeaveEdges)
					{
						//extra zig
						if((((!SquareIsWall(rightx4))&&(SquareIsWall(rightx5))&&(!NodeExists(leftx2)))||
						    ((!SquareIsWall(leftx4))&&(SquareIsWall(leftx5))&&(!NodeExists(rightx2))))&&
							(!SquareIsWall(forwardx1)))
						{
							movingacross = true;
							ZigZagFinalTurn();
							ToggleZigZagFinalTurn();
							movingup = false;
							movingupCounter = 0;
							zigzagCounter = 0;	
						}
						else
						{
							ToggleZigZagFinalTurn();
							ZigZagFinalTurn();
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							DQ.push_back(act(STEP));
							ZigZagFinalTurn();
							movingup = false;
							State = zRETURN;
						}
					
					}//x2 spaces left, extra zig
					else if(!SquareIsWall(forwardx1) && ((!NodeExists(rightx2))||(!NodeExists(leftx2))) && !LeaveEdges)
					{
						movingacross = true;
						ZigZagFinalTurn();
						ToggleZigZagFinalTurn();
						movingup = false;
						movingupCounter = 0;
						zigzagCounter = 0;	
					}
					else
					{
						ToggleZigZagFinalTurn();
						ZigZagFinalTurn();
						ZigZagFinalTurn();
						movingup = false;
						State = zRETURN;
					}

				}
				else //keep moving up
				{
					DQ.push_back(act(STEP));
					movingupCounter++;
				}

			}//ELSE STEP FORWARD
			else
			{
				zigzagCounter++;
				DQ.push_back(act(STEP));
			}
			break;
		}
	case zRETURN:
		{
			returning = true;
			movingup = false;
			movingacross = false;
			movingupCounter = 0;
			zigzagCounter = 0;
			square forwardx1 = FindCoordinates(FORWARD,1);
			square forwardx2 = FindCoordinates(FORWARD,2);

			//if having finished first zig, reset zigzag
			if(firstZig||secondZig)
			{
				int step = Pos.x-3;
				for(int i = 0; i < step; i++)
					DQ.push_back(act(STEP));

				ZigZagFinal(p,true);
				turnright = false;
				State = zZIGZAG;
				movingup = true;
				firstZig = true;
				movingacross = false;
				reversedZig = true;
				ZigZagFinal(p,false);
				return;
			}
			//if a reversed zig and return
			if(reversedZig)
			{
				PathChosen = CLEANUP;
				CleanUp(p,true);
				return;
			}
			//if started up the other end, zigzag has finished
			if(movingFromLeftWall && !LeaveEdges)
			{
				PathChosen = CLEANUP;
				CleanUp(p,true);
				return;
			}
						
			//if started up the other end, but edges left
			if(movingFromLeftWall && LeaveEdges)
			{
				if(!NodeExists(forwardx2))
				{
					DQ.push_back(act(STEP));
				}
				else
				{
					PathChosen = CLEANUP;
					CleanUp(p,true);
					return;
				}
			}

			//If state is return, move forward until a wall is x2 spaces ahead and turn, set state to zigzag
			if(SquareIsWall(forwardx1)||SquareIsWall(forwardx2))
			{
				returned = true;
				returning = false;
				movingacross = true;
				ZigZagFinalTurn();
				State = zZIGZAG;
			}
			else
				DQ.push_back(act(STEP));
			break;
		}
	case zSTART:
		{	
			State = zZIGZAG;
			square forward = FindCoordinates(FORWARD,1);
			square back = FindCoordinates(BACK,1);
			square left = FindCoordinates(LEFT,1);
			square right = FindCoordinates(RIGHT,1);
			
			bool leftWall = SquareIsWall(left);
			bool rightWall = SquareIsWall(right);
			bool forWall = SquareIsWall(forward);
			bool backWall = SquareIsWall(back);

			//TOP LEFT CORNER
			if(leftWall && forWall)
			{
				startingFromLeftWall = true;
				DQ.push_back(act(TURNRIGHT));
				movingacross = false;
				movingup = true;

			} //TOP RIGHT CORNER
			else if(rightWall && forWall)
			{
				firstZig = false;
				State = zRETURN;
				ToggleZigZagFinalTurn();
				DQ.push_back(act(TURNLEFT));

			} //BOTTOM LEFT CORNER
			else if(leftWall && backWall)
			{
				startingFromLeftWall = true;
				ZigZag(p,false);

			} //BOTTOM RIGHT CORNER
			else if(rightWall && backWall)
			{
				ToggleZigZagFinalTurn();
				ZigZag(p,false);
				//PathChosen = ZIGZAG;
				//ZigZag(p,false);
				return;

			} //BOTTOM
			else if(backWall)
			{
				ZigZag(p,false);
			
			} //RIGHT WALL
			else if(rightWall)
			{
				ToggleZigZagFinalTurn();
				ZigZag(p,false);
				//PathChosen = ZIGZAG;
				//ZigZag(p,false);
				return;
				
			} //LEFT WALL
			else if(leftWall)
			{
				startingFromLeftWall = true;
				ZigZag(p,false);

			} //TOP
			else if(forWall)
			{
				DQ.push_back(act(TURNRIGHT));
				movingup = true;
				movingacross = false;
			}
			else
			{
				ZigZag(p,false);
			}
			break;
		}
	}
}
