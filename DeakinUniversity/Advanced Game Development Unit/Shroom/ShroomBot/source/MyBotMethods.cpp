
#include "MyBot.h"
int MyBot::trialNo = 0;

//-----------------------------------------------------------------------------------------------
//Check if node already exists in map
//-----------------------------------------------------------------------------------------------
bool MyBot::NodeExists(string key)
{
	ShroomMap::iterator it = Map.find(key);
	ShroomMap::iterator end = Map.end();
	if(it == end)
		return false;
	else
		return true;
}
bool MyBot::NodeExists(square & coordinates)
{
	string key = K(coordinates);
	ShroomMap::iterator it = Map.find(key);
	ShroomMap::iterator end = Map.end();
	if(it == end)
		return false;
	else
		return true;
}
//-----------------------------------------------------------------------------------------------
//Keygen returns key for use in map
//-----------------------------------------------------------------------------------------------
string MyBot::K(int x, int y)
{
	if((y > -10)&&(y < 10)&&(x > -10)&&(x < 10))
		ss << 0 << x << 0 << y;
	if((x > -10)&&(x < 10))
		ss << 0 << x << y;
	if((y > -10)&&(y < 10))
		ss << x << 0 << y;
	else
		ss << x << y;
	string temp = ss.str();
	ss.seekp(0);
	ss.str("");
	return temp;
}
string MyBot::K(square & x)
{
	return K(x.x,x.y);
}
//-----------------------------------------------------------------------------------------------
//Create surrounding nodes
//-----------------------------------------------------------------------------------------------
void MyBot::CreateSurroundingNodes(const PERCEPT &p)
{
	square up(Pos.x,Pos.y+1);
	square down(Pos.x,Pos.y-1);
	square left(Pos.x-1,Pos.y);
	square right(Pos.x+1,Pos.y);

	if(Orientation == oUP)
	{
		CreateNewNode(p.view[FORWARD],up.x,up.y);
		CreateNewNode(p.view[RIGHT],right.x,right.y);
		CreateNewNode(p.view[LEFT],left.x,left.y);
	}
	else if(Orientation == oDOWN)
	{
		CreateNewNode(p.view[FORWARD],down.x,down.y);
		CreateNewNode(p.view[RIGHT],left.x,left.y);
		CreateNewNode(p.view[LEFT],right.x,right.y);
	}
	else if(Orientation == oLEFT)
	{
		CreateNewNode(p.view[FORWARD],left.x,left.y);
		CreateNewNode(p.view[RIGHT],up.x,up.y);
		CreateNewNode(p.view[LEFT],down.x,down.y);
	}
	else if(Orientation == oRIGHT)
	{
		CreateNewNode(p.view[FORWARD],right.x,right.y);
		CreateNewNode(p.view[RIGHT],down.x,down.y);
		CreateNewNode(p.view[LEFT],up.x,up.y);
	}
}
//-----------------------------------------------------------------------------------------------
//Create node and insert into map
//-----------------------------------------------------------------------------------------------
void MyBot::CreateNewNode(int type, int x, int y)
{
	string key = K(x,y);
	if(NodeExists(key))
	{
		//if node already exists, update it 
		Map[key].type = type;
		return;
	}
	node newNode;
	square newSquare(x,y);
	newNode.type = type;
	newNode.coord = newSquare;
	newNode.virtualWall = false;
	Map.insert(MapPair(key,newNode));
}
//-----------------------------------------------------------------------------------------------
//Get node based on action given
//-----------------------------------------------------------------------------------------------
node* MyBot::FindNode(int SearchDirection, int SpacesAway)
{
	square coordinates = FindCoordinates(SearchDirection,SpacesAway);
	//Find node from coordinates
	if(NodeExists(K(coordinates)))
		return &Map[K(coordinates)];
	else
		return NULL;
}
node* MyBot::FindNode(square & coordinates)
{
	//Find node from coordinates
	if(NodeExists(K(coordinates)))
		return &Map[K(coordinates)];
	else
		return NULL;
}
//-----------------------------------------------------------------------------------------------
//Find coordinates
//-----------------------------------------------------------------------------------------------
square MyBot::FindCoordinates(int SearchDirection, int SpacesAway)
{
	square coordinates;
	//square upwards
	if(((Orientation == oUP)&&(SearchDirection == FORWARD))||
	   ((Orientation == oLEFT)&&(SearchDirection == RIGHT))||
	   ((Orientation == oRIGHT)&&(SearchDirection == LEFT))||
	   ((Orientation == oDOWN)&&(SearchDirection == BACK)))
	{
		coordinates.x = Pos.x;
		coordinates.y = Pos.y + SpacesAway;

	} //square downwards
	else if(((Orientation == oDOWN)&&(SearchDirection == FORWARD))||
			((Orientation == oLEFT)&&(SearchDirection == LEFT))||
			((Orientation == oRIGHT)&&(SearchDirection == RIGHT))||
			((Orientation == oUP)&&(SearchDirection == BACK)))
	{
		coordinates.x = Pos.x;
		coordinates.y = Pos.y - SpacesAway;

	} //square left
	else if(((Orientation == oLEFT)&&(SearchDirection == FORWARD))||
			((Orientation == oUP)&&(SearchDirection == LEFT))||
			((Orientation == oDOWN)&&(SearchDirection == RIGHT))||
			((Orientation == oRIGHT)&&(SearchDirection == BACK)))
	{
		coordinates.x = Pos.x - SpacesAway;
		coordinates.y = Pos.y;

	} //square right
	else if(((Orientation == oRIGHT)&&(SearchDirection == FORWARD))||
			((Orientation == oUP)&&(SearchDirection == RIGHT))||
			((Orientation == oDOWN)&&(SearchDirection == LEFT))||
			((Orientation == oLEFT)&&(SearchDirection == BACK)))
	{
		coordinates.x = Pos.x + SpacesAway;
		coordinates.y = Pos.y;
	}
	else
	{ //BAD!!
		exit(EXIT_FAILURE);
	}
	
	return coordinates;
}
//-----------------------------------------------------------------------------------------------
//Find coordinates [SPECIALISED]
//-----------------------------------------------------------------------------------------------
square MyBot::FindCoordinates(int SearchDirection, int SpacesAway, square & position, int direction)
{
	if(direction == -100)
		direction = Orientation;

	square coordinates;
	//square upwards
	if(((direction == oUP)&&(SearchDirection == FORWARD))||
	   ((direction == oLEFT)&&(SearchDirection == RIGHT))||
	   ((direction == oRIGHT)&&(SearchDirection == LEFT))||
	   ((direction == oDOWN)&&(SearchDirection == BACK)))
	{
		coordinates.x = position.x;
		coordinates.y = position.y + SpacesAway;

	} //square downwards
	else if(((direction == oDOWN)&&(SearchDirection == FORWARD))||
			((direction == oLEFT)&&(SearchDirection == LEFT))||
			((direction == oRIGHT)&&(SearchDirection == RIGHT))||
			((direction == oUP)&&(SearchDirection == BACK)))
	{
		coordinates.x = position.x;
		coordinates.y = position.y - SpacesAway;

	} //square left
	else if(((direction == oLEFT)&&(SearchDirection == FORWARD))||
			((direction == oUP)&&(SearchDirection == LEFT))||
			((direction == oDOWN)&&(SearchDirection == RIGHT))||
			((direction == oRIGHT)&&(SearchDirection == BACK)))
	{
		coordinates.x = position.x - SpacesAway;
		coordinates.y = position.y;

	} //square right
	else if(((direction == oRIGHT)&&(SearchDirection == FORWARD))||
			((direction == oUP)&&(SearchDirection == RIGHT))||
			((direction == oDOWN)&&(SearchDirection == LEFT))||
			((direction == oLEFT)&&(SearchDirection == BACK)))
	{
		coordinates.x = position.x + SpacesAway;
		coordinates.y = position.y;
	}
	else
	{ //BAD!!
		exit(EXIT_FAILURE);
	}
	
	return coordinates;
}
//-----------------------------------------------------------------------------------------------
//Generate a Queue of directions
//-----------------------------------------------------------------------------------------------
void MyBot::GenerateActions(const PERCEPT &p)
{
	//CHECK FOR TRAPS
	if(DynamicPath && !WantMushrooms)
	{
		if(CheckForTraps(p))
			return;
	}
	if(SafetyNet())
	{
		ClearAllQueues();
		TargetObscured = true;
	}

	if(DQ.empty()) //if queue is empty: determine the next set of actions
	{
		switch(PathChosen)
		{
			case ZIGZAG: 
				ZigZag(p,false); 
				break;
			case CLEANUP: 
				CleanUp(p,false); 
				break;
			case OUTERSPIRAL: 
				OuterSpiral(p,false); 
				break;
			case INNERSPIRAL: 
				InnerSpiral(p,false); 
				break;
			case PREDICTION: 
				Prediction(p,false); 
				break;
			case ZIGZAGFINAL: 
				ZigZagFinal(p,false); 
				break;
		}
	}
}
//-----------------------------------------------------------------------------------------------
//Reset all the queues
//-----------------------------------------------------------------------------------------------
void MyBot::ClearAllQueues()
{
	DQ.clear();
	EQ.clear();
	PQ.clear();
	OQ.clear();
}
//-----------------------------------------------------------------------------------------------
//Update the world from the last tick's action
//-----------------------------------------------------------------------------------------------
void MyBot::UpdateHunterFromLastTick()
{
	//if hunter stepped but didn't go anywhere
	if((LastHealth == Health)&&(NextAction.action == STEP))
	{
		LastPos = Pos;
		LastAction = NextAction;
		return;
	}

	//if hunter's action actually worked
	LastPos = Pos;

	//STEPPING
	if(NextAction.action == STEP)
	{
		//change pos occording to orientation
		if(Orientation == oUP)
			Pos.y = Pos.y+1;
		else if(Orientation == oDOWN)
			Pos.y = Pos.y-1;
		else if(Orientation == oLEFT)
			Pos.x = Pos.x-1;
		else if(Orientation == oRIGHT)
			Pos.x = Pos.x+1;
	} //ROTATING
	else if((NextAction.action == TURNLEFT)||(NextAction.action == TURNRIGHT)) 
	{
		//change orientation
		if(Orientation == oUP)
		{
			if(NextAction.action == TURNLEFT)
				Orientation = oLEFT;
			if(NextAction.action == TURNRIGHT)
				Orientation = oRIGHT;
		}
		else if(Orientation == oDOWN)
		{
			if(NextAction.action == TURNLEFT)
				Orientation = oRIGHT;
			if(NextAction.action == TURNRIGHT)
				Orientation = oLEFT;
		}
		else if(Orientation == oLEFT)
		{
			if(NextAction.action == TURNLEFT)
				Orientation = oDOWN;
			if(NextAction.action == TURNRIGHT)
				Orientation = oUP;
		}
		else if(Orientation == oRIGHT)
		{
			if(NextAction.action == TURNLEFT)
				Orientation = oUP;
			if(NextAction.action == TURNRIGHT)
				Orientation = oDOWN;
		}	
	} 
	//save values
	LastAction = NextAction;
}
//-----------------------------------------------------------------------------------------------
//Find the new orientation given an orientation and change
//-----------------------------------------------------------------------------------------------
int MyBot::ChangeOrientation(int currentOrientation, int direction)
{
	//change currentOrientation
	if(currentOrientation == oUP)
	{
		if(direction == LEFT)
			currentOrientation = oLEFT;
		else if(direction == RIGHT)
			currentOrientation = oRIGHT;
		else if(direction == BACK)
			currentOrientation = oDOWN;
		else if(direction == FORWARD)
			currentOrientation = oUP;
	}
	else if(currentOrientation == oDOWN)
	{
		if(direction == LEFT)
			currentOrientation = oRIGHT;
		else if(direction == RIGHT)
			currentOrientation = oLEFT;
		else if(direction == BACK)
			currentOrientation = oUP;	
		else if(direction == FORWARD)
			currentOrientation = oDOWN;	
	}
	else if(currentOrientation == oLEFT)
	{
		if(direction == LEFT)
			currentOrientation = oDOWN;
		else if(direction == RIGHT)
			currentOrientation = oUP;
		else if(direction == BACK)
			currentOrientation = oRIGHT;
		else if(direction == FORWARD)
			currentOrientation = oLEFT;
	}
	else if(currentOrientation == oRIGHT)
	{
		if(direction == LEFT)
			currentOrientation = oUP;
		else if(direction == RIGHT)
			currentOrientation = oDOWN;
		else if(direction == BACK)
			currentOrientation = oLEFT;
		else if(direction == FORWARD)
			currentOrientation = oRIGHT;
	}	
	return currentOrientation;
}
//-----------------------------------------------------------------------------------------------
//Get the next action depending on which queue
//-----------------------------------------------------------------------------------------------
act MyBot::GetNextAction()
{
	if(PQ.empty())
		return DQ[0];
	else
		return PQ[0]; 
}
void MyBot::PopNextAction()
{
	if(PQ.empty())
		DQ.pop_front();
	else
		PQ.pop_front();
}
//-----------------------------------------------------------------------------------------------
//Generate magnitude of a vector
//-----------------------------------------------------------------------------------------------
float MyBot::M(int x, int y)
{
	float temp = float((x*x)+(y*y));
	return sqrt(temp);
}
float MyBot::M(vec2D & v)
{
	return M(v.x,v.y);
}
//-----------------------------------------------------------------------------------------------
//For the first tick
//-----------------------------------------------------------------------------------------------
bool MyBot::OnWorldEntry(const PERCEPT &p)
{
	//create initial nodes
	CreateNewNode(EMPTY,Pos.x,Pos.y);
	CreateNewNode(p.view[FORWARD],Pos.x,Pos.y+1);
	CreateNewNode(p.view[BACK],Pos.x,Pos.y-1);
	CreateNewNode(p.view[RIGHT],Pos.x+1,Pos.y);
	CreateNewNode(p.view[LEFT],Pos.x-1,Pos.y);

	//reset paths
	ZigZag(p,true);
	OuterSpiral(p,true);
	InnerSpiral(p,true);
	ZigZagFinal(p,true); 
	if(PathChosen == PREDICTION)
		Prediction(p,true);

	//hunter
	LastAction.action = QUIT;
	NextAction.action = QUIT;

	return false;
}
//-----------------------------------------------------------------------------------------------
//Slow down the bot
//-----------------------------------------------------------------------------------------------
void MyBot::SlowBot(clock_t delay)
{ 
		clock_t start = clock(); //set the start of the time
		while ((clock() - start) < delay ) // calculate the difference
			continue;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

char const* MyBot::name() const
{
	return "Jensen"; // Insert student surname here
}
char const* MyBot::id() const
{
	return "900081386"; // insert student id number here
}
void MyBot::Attach(LPCONTROLLABLE const& pA)
{
	m_pAvatar = pA;
}



