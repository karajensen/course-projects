#include "MyBot.h"
//-----------------------------------------------------------------------------------------------
//OUTERSPIRAL
//-----------------------------------------------------------------------------------------------
enum {sSTART = 0, sSPIRAL, sWALL, sWALLFIX, sSPIRALFIX};
void MyBot::OuterSpiral(const PERCEPT &p, bool reset)
{
	static int State;
	static int stepsNeeded;
	static int stepsTaken;
	static int corner;
	static bool turnright;
	static int initialsteps;
	static int Startcounter;
	static int Startcounter2;
	static int Wallcounter;
	static int SFixcounter;

	if(reset)
	{
		State = sSTART;
		stepsNeeded = 7;
		stepsTaken = 0;
		corner = 1;
		turnright = true;
		initialsteps = 2;
		Startcounter = 0;
		Startcounter2 = 0;
		Wallcounter = 0;
		SFixcounter = 0;
		return;
	}

	//Move until reaching a square that's not in no-go zone, if hit a wall before then turn and contiune
	//Pattern: 2 5 7 10 10 13 13....
	switch(State)
	{
	case sSTART:
		{
			if((p.view[FORWARD] == WALL)&&(Startcounter < initialsteps))
			{
				//if a wall breaks the spiral before the initial steps are completed
				//this usually means hunter has spawned close to a wall(s)
				PathChosen = INNERSPIRAL;
				GenerateActions(p);
				break;
			}
			if(Startcounter < initialsteps)
			{
				DQ.push_back(act(STEP));//step for two/five paces
				Startcounter++;
			} 
			else
			{
				DQ.push_back(act(TURNRIGHT));
				Startcounter = 0;
				initialsteps = 5;
				Startcounter2++;
				if(Startcounter2 >= 2)
					State = sSPIRAL;
			}
			break;
		}
	case sSPIRAL:
		{
			if((p.view[FORWARD] == WALL)&&(stepsTaken < stepsNeeded))
			{
				State = sWALL;
				OuterSpiral(p,false);
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
			if((p.view[FORWARD])&&(p.view[RIGHT]||p.view[LEFT])) 
			{
				//IF STUCK IN CORNER [TO FIX]
				PathChosen = CLEANUP;
				GenerateActions(p);
				break;
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
			if((p.view[FORWARD] == WALL)&&(Wallcounter < 2))
			{
				Wallcounter = 0;
				State = sWALL;
				OuterSpiral(p,false);
			}
			if(Wallcounter < 2)
			{
				DQ.push_back(act(STEP));//step for two paces
				Wallcounter++;
			} 
			else
			{
				if(turnright){DQ.push_back(act(TURNRIGHT));}
				else {DQ.push_back(act(TURNLEFT));}	
				State = sSPIRALFIX;
				Wallcounter = 0;
			}
			break;
		}
	case sSPIRALFIX: //Move around spiral in opposite direction
		{
			if((p.view[FORWARD] == WALL)&&(SFixcounter < (stepsTaken+2)))
			{
				SFixcounter = 0;
				State = sWALL;
				OuterSpiral(p,false);
				return;
			}
			if(SFixcounter < (stepsTaken+2))
			{
				DQ.push_back(act(STEP));  //go up amount actually done+3
				SFixcounter++;
			}
			else
			{
				if(turnright){DQ.push_back(act(TURNRIGHT));}
				else {DQ.push_back(act(TURNLEFT));}	
				stepsTaken = 0;
				stepsNeeded = stepsNeeded + 6;
				corner = 1;
				State = sSPIRAL;
				SFixcounter = 0;
			}
			break;
		}
	}
}
