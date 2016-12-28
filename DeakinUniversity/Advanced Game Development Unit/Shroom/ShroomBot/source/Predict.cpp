#include "MyBot.h"

//-----------------------------------------------------------------------------------------------
//Generate actions for prediction path
//-----------------------------------------------------------------------------------------------
void MyBot::Prediction(const PERCEPT &p, bool reset)
{
	if(reset)
	{	
		//CHECK IF MAP NEEDS TO BE SAVED
		if(SaveMSD) 
			SaveMSDtoFile();
		return;
	}

	//RESET IF TARGET OBSCURED
	if(TargetObscured)
	{
		TargetObscured = false;
		EQ.clear();
	}

	//GATHERING DATA
	if((SaveMSD)&&(trialNo <= 1000))
	{
		CleanUp(p,false);
		return;
	}
	
	//CREATE PATH
	if(!SaveMSD)
	{
		if(EQ.empty())
		{
			if(!FindPredictionPath())
			{
				//FAILED
				int r = rand() % 6; //0 to 5
				if(r <= 3)
					DQ.push_back(act(STEP));
				else if(r == 4)
					DQ.push_back(act(TURNRIGHT));
				else if(r == 5)
					DQ.push_back(act(TURNLEFT));
			}
		}
		act nextAction = EQ[0];
		EQ.pop_front();
		DQ.push_back(nextAction);
	}
}
//-----------------------------------------------------------------------------------------------
//Find the next target
//-----------------------------------------------------------------------------------------------
bool MyBot::FindPredictionPath()
{
	return (SearchForTarget());

	//Get surrounding coords (forward, right, left, back)
	/*square f = FindCoordinates(FORWARD,2);
	square l = FindCoordinates(LEFT,2);
	square r = FindCoordinates(RIGHT,2);
	square b = FindCoordinates(BACK,2);
	square fx1 = FindCoordinates(FORWARD,1);
	square lx1 = FindCoordinates(LEFT,1);
	square rx1 = FindCoordinates(RIGHT,1);
	square bx1 = FindCoordinates(BACK,1);

	//Get values of squares
	float fVal = GetSquareValue(f);
	float rVal = GetSquareValue(r);
	float lVal = GetSquareValue(l);
	float bVal = GetSquareValue(b);

	//check for walls
	if(SquareIsWall(f))
		fVal = -100;
	if(SquareIsWall(b))
		bVal = -100;
	if(SquareIsWall(r))
		rVal = -100;
	if(SquareIsWall(l))
		lVal = -100;

	//check for mushrooms
	if(!WantMushrooms)
	{
		if(SquareIsMushroom(fx1))
			fVal = -100;
		if(SquareIsMushroom(rx1))
			rVal = -100;
		if(SquareIsMushroom(lx1))
			lVal = -100;
		if(SquareIsMushroom(bx1))
			bVal = -100;
	}

	//choose best value over four values
	BestChoice = max(bVal,lVal);
	BestChoice = max(BestChoice,rVal);
	BestChoice = max(BestChoice,fVal);

	//if the bestchoice is a seen node
	if((BestChoice < 0)||(LastBestChoice > BestChoice))
		return (SearchForTarget());

	LastBestChoice = BestChoice;

	//Target found
	if(BestChoice == fVal)
	{
		EQ.push_back(act(STEP));
	}
	else if(BestChoice == rVal)
	{
		//check square one down from the right exists
		square oneDownFromRight = FindCoordinates(BACK,1,r);
		square fx1 = FindCoordinates(FORWARD,1);

		//if node exists, take an x2 extra step if x2 forward isn't a wall
		if(NodeExists(oneDownFromRight)&&!SquareIsWall(fx1))
			EQ.push_back(act(STEP));
		
		EQ.push_back(act(TURNRIGHT));
		EQ.push_back(act(STEP));
	}
	else if(BestChoice == lVal)
	{
		//check square one down from the left exists
		square oneDownFromLeft = FindCoordinates(BACK,1,l);
		square fx1 = FindCoordinates(FORWARD,1);

		//if node exists, take an x2 extra step if x2 forward isn't a wall
		if(NodeExists(oneDownFromLeft)&&!SquareIsWall(fx1))
			EQ.push_back(act(STEP));
		EQ.push_back(act(TURNLEFT));
		EQ.push_back(act(STEP));
	}
	else if(BestChoice == bVal)
	{
		EQ.push_back(act(TURNLEFT));
		EQ.push_back(act(TURNLEFT));
		EQ.push_back(act(STEP));
	}
	return true;*/
}

//-----------------------------------------------------------------------------------------------
//Choose the next target by the best value
//-----------------------------------------------------------------------------------------------
bool MyBot::SearchForTarget()
{
	//Find a target coordinate with required probability
	//that doen't yet exist and isn't a wall
	
	//set up search
	float probability;
	SquareContainer SearchedSqrs;
	static int savedSearchDepth = 1;
	int SearchDepth = 1;
	int MaxSearchDepth = max(MapColumns,MapRows);
	bool Found = false;

	//Get Probability to search
	int tempSI = SearchIndex;

	while((!Found)&&(tempSI >= 0))
	{
			probability = MSD[tempSI].probability;
			if(Orientation == oUP)
			{
				Found = SearchForBestSquare(pY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
			}
			else if(Orientation == oDOWN)
			{
				Found = SearchForBestSquare(nY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
			}
			else if(Orientation == oRIGHT)
			{
				Found = SearchForBestSquare(pX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
			}
			else if(Orientation == oLEFT)
			{
				Found = SearchForBestSquare(nX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(nY, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
				Found = SearchForBestSquare(pX, SearchedSqrs, SearchDepth, probability);
				if(Found){break;}
			}

			SearchDepth++;
			if(SearchDepth >= MaxSearchDepth)
			{
				tempSI = tempSI-2;
				SearchDepth = 1;
			}
	}

	if(!Found)
		return false;
	else
	{
		PredictionTarget = SearchedSqrs[SearchedSqrs.size()-1];
		//target square found, move towards it
		if(DynamicPath)
			FindBestPath(PredictionTarget,EQ);
		else
			OldCleanupPath(PredictionTarget,EQ);
			
		LastBestChoice = GetSquareValue(PredictionTarget);
	}
	return true;
}
//-----------------------------------------------------------------------------------------------
//Search left/right of node for best nodes
//-----------------------------------------------------------------------------------------------
bool MyBot::SearchForBestSquare(int direction, SquareContainer & SearchedSqrs, int SearchDepth, float valueToFind)
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
	float vposVal = GetSquareValue(vPos);
	if((valueToFind <= vposVal)&&(!SquareIsWall(vPos)))
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
			float rvVal = GetSquareValue(rvPos);
			if((valueToFind <= rvVal)&&(!SquareIsWall(rvPos)))
				return true;
		}
		//if square hasn't been checked before
		if(!lvFound)
		{
			SearchedSqrs.push_back(lvPos);
			//check if a target square
			float lvVal = GetSquareValue(lvPos);
			if((valueToFind <= lvVal)&&(!SquareIsWall(lvPos)))
				return true;
		}
	}
	//if searched all side nodes and nothing found, return
	return false;
}