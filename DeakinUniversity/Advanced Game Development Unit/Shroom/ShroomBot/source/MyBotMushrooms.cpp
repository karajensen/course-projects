
#include "MyBot.h"

//-----------------------------------------------------------------------------------------------
//calculate mushroom probabilities for the entire distribution
//-----------------------------------------------------------------------------------------------
void MyBot::CalculateMushroomProbability()
{
	//MushroomDistribution[MushroomTypeNo-1].count = MushroomDistribution[MushroomTypeNo-1].count - 1;
	//if(MushroomDistribution[MushroomTypeNo-1].count == 0)
	//	MushroomTypeNo--;

	float OverallMushroomCount = 0;
	for(int i = 0; i < MushroomTypeNo; i++)
		OverallMushroomCount += MushroomDistribution[i].count;

	ExpectedGain = 0;
	ActualGain = 0;
	for(int i = 0; i < MushroomTypeNo; i++) //for every mushroom type
	{
		MushroomDistribution[i].probability = MushroomDistribution[i].count/OverallMushroomCount; //probability=count/overall
		MushroomDistribution[i].probxvalue = MushroomDistribution[i].probability*MushroomDistribution[i].value; //probability*value
		ExpectedGain += MushroomDistribution[i].probxvalue;
	}
	LikelyGain = ExpectedGain;
}
//-----------------------------------------------------------------------------------------------
//Calculate the actual gain from the mushrooms eaten so far and compare to expected gain
//-----------------------------------------------------------------------------------------------
void MyBot::UpdateMushroomActualGain()
{
	//Find which mushroom it was from distribution
	int HealthDiff = Health-LastHealth;
	for(unsigned int i = 0; i < MushroomDistribution.size(); i++)
	{
		if(HealthDiff == MushroomDistribution[i].value)
		{
			//add value of mushroom to actual gain
			ActualGain += MushroomDistribution[i].probxvalue;
			break;
		}
	}
	/*//If number of mushrooms left is 1/3 and actual gain is higher than expected, stop eating
	if((CurrentMushroomNo <= MinMushroomNo) && DynamicPath)
	{	
		if(ActualGain > ExpectedGain)
			WantMushrooms = false;
		else
			WantMushrooms = true;
	}*/

	LikelyGain = ExpectedGain-ActualGain;
}
//-----------------------------------------------------------------------------------------------
//Check for Mushrooms Traps
//-----------------------------------------------------------------------------------------------
bool MyBot::CheckForTraps(const PERCEPT &p)
{
	bool fTrap = false;
	bool lTrap = false;
	bool rTrap = false;
	if((p.view[FORWARD] == SHROOM)||(p.view[FORWARD] == WALL))
		fTrap = true;
	if((p.view[RIGHT] == SHROOM)||(p.view[RIGHT] == WALL))
		rTrap = true;
	if((p.view[LEFT] == SHROOM)||(p.view[LEFT] == WALL))
		lTrap = true;

	if(fTrap && rTrap && lTrap)
	{
		if(p.view[FORWARD] == SHROOM)
		{
			node* ShroomNode = FindNode(FORWARD,1);
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
		}
		else if(p.view[RIGHT] == SHROOM)
		{	
			node* ShroomNode = FindNode(RIGHT,1);
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
			AddToPriorityQueue(act(TURNRIGHT));
		}
		else if(p.view[LEFT] == SHROOM)
		{	
			node* ShroomNode = FindNode(LEFT,1);
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
			AddToPriorityQueue(act(TURNLEFT));
		}
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------
//Check for Mushrooms
//-----------------------------------------------------------------------------------------------
void MyBot::CheckMushrooms(const PERCEPT &p)
{
	//DynamicPath = false; //whether to go around mushrooms or not
	//WantMushrooms = false; //whether to eat mushrooms or not


	//SHROOM FORWARD; Next move is a step
	if((p.view[FORWARD] == SHROOM)&&(NextAction.action == STEP))
	{
		node* ShroomNode = FindNode(FORWARD,1);
		if(!DynamicPath)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
		}
		else if(WantMushrooms && DynamicPath)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
		}
		else
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));

			/*//Check if mushroom is in a trap
			square forward = FindCoordinates(FORWARD,1);
			square fxr = FindCoordinates(RIGHT,1,forward);
			square fxl = FindCoordinates(LEFT,1,forward);
			if(SquareIsMushroom(forward)&&SquareIsMushroom(fxr)&&SquareIsMushroom(fxl))
			{
				EatMushroom(ShroomNode);
				AddToPriorityQueue(act(EAT));
			}
			else
			{
				//if not a trap, waste a turn to find 
				DQ.push_back(act(STEP));
			}*/
		}

	} //SHROOM FORWARD; Next move isn't a step
	else if((p.view[FORWARD] == SHROOM)&&(NextAction.action != STEP))
	{
		node* ShroomNode = FindNode(FORWARD,1);
		if(WantMushrooms)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(EAT));
		}

	} //SHROOM RIGHT AND RIGHT ISN'T NEXT ACTION
	else if((p.view[RIGHT] == SHROOM)&&(NextAction.action != TURNRIGHT))
	{ 
		node* ShroomNode = FindNode(RIGHT,1);
		if(WantMushrooms)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(TURNLEFT));
			AddToPriorityQueue(act(EAT));
			AddToPriorityQueue(act(TURNRIGHT));
		}

	} //SHROOM TO LEFT; NEXT MOVE ISN'T LEFT
	else if((p.view[LEFT] == SHROOM)&&(NextAction.action != TURNLEFT))
	{
		float CostToEat = float(2*TurnCost);
		float CostToGoAround = 0;
		node* ShroomNode = FindNode(LEFT,1);
		
		if(WantMushrooms)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(TURNRIGHT));
			AddToPriorityQueue(act(EAT));
			AddToPriorityQueue(act(TURNLEFT));
		}

	} //SHROOM BEHIND AND THE NEXT ACTION IS A STEP FORWARD
	else if((p.view[BACK] == SHROOM)&&(NextAction.action == STEP))
	{
		node* ShroomNode = FindNode(BACK,1);	
		if(WantMushrooms)
		{
			EatMushroom(ShroomNode);
			AddToPriorityQueue(act(TURNLEFT));
			AddToPriorityQueue(act(TURNLEFT));
			AddToPriorityQueue(act(EAT));
			AddToPriorityQueue(act(TURNLEFT));
			AddToPriorityQueue(act(TURNLEFT));
		}
	}
}
//-----------------------------------------------------------------------------------------------
//Add to priority queue
//-----------------------------------------------------------------------------------------------
void MyBot::AddToPriorityQueue(act newAction)
{
	//add new action
	PQ.push_front(newAction);

	//Check for tripling of actions; lll is mapped to r, rrr is mapped to l
	int PQsize = PQ.size();
	if(PQsize > 2) //if there's more than three actions queue
	{
		//Store actions for easy checking
		string toCheck = "";
		for(int i = 0; i < PQsize; i++)
		{
			if(PQ[i].action == TURNRIGHT)
				toCheck += "r";
			else if(PQ[i].action == TURNLEFT)
				toCheck += "l";
			else if(PQ[i].action == EAT)
				toCheck += "e";
			else
				toCheck += "b"; //should not get this!
		}
		int occurance = 0;
		string rrr = "rrr";
		while(occurance != string::npos) //loop until all checked
		{
			occurance = toCheck.find(rrr); // returning first occurance's index or string::npos
			if(occurance != string::npos)
			{
				PQ.insert(PQ.begin()+occurance,act(TURNLEFT));
				ActionQueueIt start = PQ.begin() + occurance + 1;
				ActionQueueIt end = PQ.begin()+ occurance + 4;
				PQ.erase(start,end);
				
				toCheck.insert(occurance, "l"); //inserts substring one before index
				string::iterator sstart = toCheck.begin()+ occurance +1;
				string::iterator ssend = toCheck.begin()+ occurance +4;
				toCheck.erase(sstart,ssend);
			}
		}
		occurance = 0;
		string lll = "lll";
		while(occurance != string::npos) //loop until all checked
		{
			occurance = toCheck.find(lll); // returning first occurance's index or string::npos
			if(occurance != string::npos)
			{
				PQ.insert(PQ.begin()+occurance,act(TURNRIGHT));
				ActionQueueIt start = PQ.begin() + occurance + 1;
				ActionQueueIt end = PQ.begin()+ occurance + 4;
				PQ.erase(start,end);
				
				toCheck.insert(occurance, "r"); //inserts substring one before index
				string::iterator sstart = toCheck.begin()+ occurance +1;
				string::iterator ssend = toCheck.begin()+ occurance +4;
				toCheck.erase(sstart,ssend);
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------
//Remove Eaten mushroom from Known Mushrooms if already in
//-----------------------------------------------------------------------------------------------
void MyBot::EatMushroom(node* ShroomToRemove)
{
	ShroomToRemove->type = EMPTY; //set node in Map to empty
	CurrentMushroomNo--;
}
//-----------------------------------------------------------------------------------------------
//Check for Magic Shroom
//-----------------------------------------------------------------------------------------------
bool MyBot::CheckMagicShroom(const PERCEPT &p, bool eat)
{
	bool MagicShroomFound = false;
	square MagicShroomCoord;
	if(p.view[FORWARD] == MAGICSHROOM)
	{
		MagicShroomCoord = FindCoordinates(FORWARD,1);
		if(eat)
		{
			m_pAvatar->QueueAction(EAT);
			m_pAvatar->ProcessQueue();
			MagicShroomFound = true;
		}
	}
	else if(p.view[LEFT] == MAGICSHROOM)
	{
		MagicShroomCoord = FindCoordinates(LEFT,1);
		if(eat)
		{
			m_pAvatar->QueueAction(TURNLEFT);
			m_pAvatar->QueueAction(EAT);
			m_pAvatar->ProcessQueue();
			MagicShroomFound = true;
		}
	}
	else if(p.view[RIGHT] == MAGICSHROOM)
	{
		MagicShroomCoord = FindCoordinates(RIGHT,1);
		if(eat)
		{
			m_pAvatar->QueueAction(TURNRIGHT);
			m_pAvatar->QueueAction(EAT);
			m_pAvatar->ProcessQueue();
			MagicShroomFound = true;
		}
	}
	else if(p.view[BACK] == MAGICSHROOM)
	{
		MagicShroomCoord = FindCoordinates(BACK,1);
		if(eat)
		{
			m_pAvatar->QueueAction(TURNRIGHT);
			m_pAvatar->QueueAction(TURNRIGHT);
			m_pAvatar->QueueAction(EAT);
			m_pAvatar->ProcessQueue();
			MagicShroomFound = true;
		}
	}

	if(MagicShroomFound)
	{
		MagicShroom = FindNode(MagicShroomCoord);
		SaveMagicShroom(MagicShroomCoord);
		return true;
	}
	else
		return false;
}
//-----------------------------------------------------------------------------------------------
//Check if a node is a mushroom
//-----------------------------------------------------------------------------------------------
bool MyBot::SquareIsMushroom(square & coord)
{
	node * MapNode = FindNode(coord);
	if(MapNode != NULL)
	{
		if(MapNode->type == SHROOM)
			return true;
	}
	return false;
}