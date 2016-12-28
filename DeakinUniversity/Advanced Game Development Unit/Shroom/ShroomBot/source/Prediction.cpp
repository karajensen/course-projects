/*#include "MyBot.h"
MagicShroomContainer MyBot::MSD;
int MyBot::MagicShroomFound = 0;
int MyBot::trialNo = 0;

//-----------------------------------------------------------------------------------------------
//Generate actions for prediction path
//-----------------------------------------------------------------------------------------------
void MyBot::Prediction(const PERCEPT &p, bool reset)
{
	if(reset)
	{
		CreatePredictionMap();
		return;
	}
	if(trialNo <= -1) //use a zigzag for first 100 trials to gather data
	{
		PathChosen = ZIGZAG;
		GenerateActions(p);
	}
	else
	{
		if(!FindPredictionPath()) //SUCCEEDED
		{
			CreatePredictedPath();
			act nextAction = EQ[0];
			EQ.pop_front();
			DQ.push_back(nextAction);
		}
		else //FAILED
		{
			OQ.clear();
		}
	}
}
//-----------------------------------------------------------------------------------------------
//A* styled algorithm for finding cheapest path to goal
//-----------------------------------------------------------------------------------------------
bool MyBot::FindPredictionPath()
{
	//Start tile
	MStile startTile;
	startTile.costSoFar = 0;
	startTile.coord.x = 0;
	startTile.coord.y = 0;
	startTile.orientation = Orientation;
	
	//Create open/closed lists
	List OpenList;
	List ClosedList;

	//add start tile to the open list
	OpenList.lst.push_back(startTile);

	//While there are nodes to process
	MStile currentTile;
	while(!OpenList.lst.empty())
	{
		//Get tile from open list with smallest total estimated cost
		currentTile = OpenList.GetMStileWithSmallestCost();

		//if it is an unvisited goal, break
		MScoord * currentMScoord = FindMScoord(currentTile.coord)
		if(currentMScoord->target && (!currentMScoord->visited))
			break;
		
		//create a list of edges to process
		deque<EDGE> edgesToProcess;
		edgesToProcess = CreateOutgoingEdges(currentTile, edgesToProcess);
		
		//Loop through each edge in container
		for(unsigned int i = 0; i < edgesToProcess.size(); i++)
		{
			//get the child tile and it's cost so far
			MScoord child;
			child.coord = edgesToProcess[i].Child;
			MStile childTile;
			int childTileCost = currentTile.costSoFar + edgesToProcess[i].edgeCost;

			//check status of child tile
			if(ClosedList.Contains(child)) 
			{
				//If child tile has already been processed
				childTile = ClosedList.Find(child); //Find the node
				if(childTile.costSoFar <= childTileCost) //If stored values smaller, skip to next edge
					continue;
				ClosedList.Remove(childTile); //Remove it from the closed list
			}
			else if(OpenList.Contains(child))
			{
				 //If child tile is queued to be processed
				childTile = OpenList.Find(child); //Find the node
				if(childTile.costSoFar <= childTileCost) //If stored values smaller, skip to next edge
					continue;
				OpenList.Remove(childTile); //Remove it from the open list
			}	
			else 
			{
				//If child tile has not been visisted create a new tile
				childTile.coord = child.coord;
			}
			//update values
			childTile.edgeFromParent = edgesToProcess[i];
			childTileStruct.costSoFar = childTileCost;

			//add/re-add it to the open list
			OpenList.lst.push_back(childTileS);
		}
		//current edges finished; add current to the closed list and remove from open list
		OpenList.Remove(currentTile);
		ClosedList.lst.push_back(currentTile);
	}
	//No more coord to process, check if at a goal node
	MScoord * currentMScoord = FindMScoord(currentTile.coord)
	if(currentMScoord->target && (!currentMScoord->visited))
	{
		//add path to SquareContainer PredictedPath
		return true;
	}
	else
	{
		return false;
	}
}
//-----------------------------------------------------------------------------------------------
//Create the outgoing edges
//-----------------------------------------------------------------------------------------------
deque<EDGE> & MyBot::CreateOutgoingEdges(MStile & currentTile, deque<EDGE> & edgesToProcess)
{
	//Save and edit current Hunter (DANGEROUS!!!)
	square SavedPos = Pos;
	int SavedOrientation = Orientation;
	Pos = currentTile.coord;
	Orientation = currentTile.orientation;

	//Get surrounding coords (forward, right, left, back)
	square f = FindCoordinates(FORWARD,1);
	square l = FindCoordinates(LEFT,1);
	square r = FindCoordinates(RIGHT,1);
	square b = FindCoordinates(BACK,1);

	//set hunter back
	Pos = SavedPos;
	Orientation = SavedOrientation;

	//create outgoing connections for directions that aren't the parent of the current node or a wall or not in pMap
	if((currentTile.edgeFromParent.Parent != f)&&(!SquareIsWall(f))&&(MScoordExists(f)))
	{	
		EDGE forward(currentTile.coord,n);
		forward.edgeCost = 1;
		edgesToProcess.push_back(forward);	
	}
	if((currentTile.edgeFromParent.Parent != l)&&(!SquareIsWall(l))&&(MScoordExists(l)))
	{	
		EDGE left(currentTile.coord,l);
		left.edgeCost = 2;
		edgesToProcess.push_back(left);	
	}			
	if((currentTile.edgeFromParent.Parent != r)&&(!SquareIsWall(r))&&(MScoordExists(r)))
	{
		EDGE right(currentTile.coord,r);
		right.edgeCost = 2;
		edgesToProcess.push_back(right);	
	}			
	if((currentTile.edgeFromParent.Parent != b)&&(!SquareIsWall(b))&&(MScoordExists(b)))
	{	
		EDGE back(currentTile.coord,b);
		back.edgeCost = 3;
		edgesToProcess.push_back(back);	
	}
	return edgesToProcess;
}
//-----------------------------------------------------------------------------------------------
//Create the path from the list of coords
//-----------------------------------------------------------------------------------------------
void MyBot::CreatePredictionPath()
{
	//use container PredictedPath to create actions and add to EQ



}
//-----------------------------------------------------------------------------------------------
//check if coordinates are within the predicted shroom distance
//-----------------------------------------------------------------------------------------------
bool MyBot::IsPredictedDistance(square & coord)
{
	int band;
	if(MSD.size() > 0)
		band = MSD[MSD.size()-1].distance;
	else
		band = HardCodedBand;

	float dis = M(coord);
	if((dis <= band)&&(dis > (band-5)))
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------
//Save the magic shroom if found
//-----------------------------------------------------------------------------------------------
void MyBot::SaveMagicShroom(square & coord)
{
	//distance from magic shroom to (0,0) hunter initial pos
	int distance = FindDistanceBand(coord);

	//check if already have band saved
	bool found = false;
	unsigned int j;
	for(j = 0; j < MSD.size(); j++)
	{
		if(MSD[j].distance == distance)
		{
			found = true;
			break;
		}
	}
	if(found)
		MSD[j].count++;
	else
	{
		MS temp(distance,1,0);
		MSD.push_back(temp);
	}
	MagicShroomFound++;
}
//-----------------------------------------------------------------------------------------------
//Calculate the magic shroom probabilities
//-----------------------------------------------------------------------------------------------
void MyBot::MagicShroomProbability()
{
	trialNo++;

	//probability = count/overall count
	for(unsigned int i = 0; i < MSD.size(); i++)
		MSD[i].probability = (float(MSD[i].count))/(float(MagicShroomFound));

	//sort in order of smallest probability
	sort(MSD.begin(), MSD.end(), MSDfunctor());
}
//-----------------------------------------------------------------------------------------------
//Create the prediction map
//-----------------------------------------------------------------------------------------------
void MyBot::CreatePredictionMap()
{
	//Create max(MapColums,MapRows)*2 dimension map container holding coordinates/distances
	int max = max(MapColumns,MapRows);
	int max2 = max*2;
	int x = -max;
	int y = -max;
	for(int i = 0; i < max2; i++)
	{
		for(int j = 0; j < max2; j++)
		{
			MScoord temp;
			square coord(x,y);
			temp.coord.x = coord.x;
			temp.coord.y = coord.y;
			temp.target = IsPredictedDistance(coord);
			temp.visited = false;
			pMap.push_back(temp);
			x++;
		}
		y++;
		x = -max;
	}
}
//-----------------------------------------------------------------------------------------------
//Create the prediction map
//-----------------------------------------------------------------------------------------------
int MyBot::FindDistanceBand(square & coord)
{
	int distance = int(M(coord));

	//convert to band; up to 50 for 100x100 board
	if(distance <= 5)
		distance = 5;
	else if(distance > 5 && distance <= 10)
		distance = 10;
	else if(distance > 10 && distance <= 15)
		distance = 15;
	else if(distance > 15 && distance <= 20)
		distance = 20;
	else if(distance > 20 && distance <= 25)
		distance = 25;
	else if(distance > 25 && distance <= 30)
		distance = 30;
	else if(distance > 30 && distance <= 35)
		distance = 35;
	else if(distance > 35 && distance <= 40)
		distance = 40;
	else if(distance > 40 && distance <= 45)
		distance = 45;
	else if(distance > 45 && distance <= 50)
		distance = 50;
	else
		distance = 55;

	return distance;
}
//-----------------------------------------------------------------------------------------------
//Find the Coord given the ID
//-----------------------------------------------------------------------------------------------
 MScoord * MyBot::FindMScoord(square & ID)
{
	for(int i = 0; i < pMap.size(); i++)
	{
		if(pMap[i].coord == ID)
			return &pMap[i];
	}
	return NULL;
}
 //-----------------------------------------------------------------------------------------------
//Check if ID exists in MScoord Container
//-----------------------------------------------------------------------------------------------
 bool MyBot::MScoordExists(square & ID)
{
	for(int i = 0; i < pMap.size(); i++)
	{
		if(pMap[i].coord == ID)
			return true;
	}
	return false;
}*/