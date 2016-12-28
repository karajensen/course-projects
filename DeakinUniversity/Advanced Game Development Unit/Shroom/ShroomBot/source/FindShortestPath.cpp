#include "MyBot.h"

//-----------------------------------------------------------------------------------------------
//Create outgoing edges given a node
//-----------------------------------------------------------------------------------------------
deque<edge> & MyBot::CreateOutgoingEdges(NodeStruct & currentNode, deque<edge> & edgesToProcess)
{
		const int MushroomPlus = 2;

		//Get surrounding coords (forward, right, left, back)
		square f = FindCoordinates(FORWARD,1,currentNode.node,currentNode.edgeFromParent.directionToChild);
		square l = FindCoordinates(LEFT,1,currentNode.node,currentNode.edgeFromParent.directionToChild);
		square r = FindCoordinates(RIGHT,1,currentNode.node,currentNode.edgeFromParent.directionToChild);
		square b = FindCoordinates(BACK,1,currentNode.node,currentNode.edgeFromParent.directionToChild);

		//create outgoing connections for directions that aren't the parent of the current node or a wall
		//or a mushroom (if don't want mushrooms)
		bool fpOK = true;
		if(!WantMushrooms && SquareIsMushroom(f))
			fpOK = false;
		if((f != currentNode.node) && !SquareIsWall(f) && fpOK)
		{	
			edge forward;
			forward.directionToChild = ChangeOrientation(currentNode.edgeFromParent.directionToChild,FORWARD);
			forward.parent = currentNode.node;
			forward.child = f;
			forward.edgeCost = -1;
			if(WantMushrooms && SquareIsMushroom(f))
				forward.edgeCost += MushroomPlus;
			edgesToProcess.push_back(forward);	
		}

		bool rpOK = true;
		if(!WantMushrooms && SquareIsMushroom(r))
			rpOK = false;
		if((r != currentNode.node) && !SquareIsWall(r) && rpOK)
		{	
			edge right;
			right.directionToChild = ChangeOrientation(currentNode.edgeFromParent.directionToChild,RIGHT);
			right.parent = currentNode.node;
			right.child = r;
			right.edgeCost = -2;
			if(WantMushrooms && SquareIsMushroom(r))
				right.edgeCost += MushroomPlus;
			edgesToProcess.push_back(right);	
		}
		bool lpOK = true;
		if(!WantMushrooms && SquareIsMushroom(l))
			lpOK = false;
		if((l != currentNode.node) && !SquareIsWall(l) && lpOK)
		{	
			edge left;
			left.directionToChild = ChangeOrientation(currentNode.edgeFromParent.directionToChild,LEFT);
			left.parent = currentNode.node;
			left.child = l;
			left.edgeCost = -2;
			if(WantMushrooms && SquareIsMushroom(l))
				left.edgeCost += MushroomPlus;
			edgesToProcess.push_back(left);	
		}

		bool bpOK = true;
		if(!WantMushrooms && SquareIsMushroom(b))
			bpOK = false;
		if((b != currentNode.node) && !SquareIsWall(b) && bpOK)
		{	
			edge back;
			back.directionToChild = ChangeOrientation(currentNode.edgeFromParent.directionToChild,BACK);
			back.parent = currentNode.node;
			back.child = b;
			back.edgeCost = -3;
			if(WantMushrooms && SquareIsMushroom(b))
				back.edgeCost += MushroomPlus;
			edgesToProcess.push_back(back);	
		}
	return edgesToProcess;
}

//-----------------------------------------------------------------------------------------------
//Generate Heuristic
//-----------------------------------------------------------------------------------------------
float MyBot::CreateHeuristic(square & coord, square & target)
{
	//Heutristic = |Grow – nrow|+|Gcol- ncol|
	float first = float(target.x-coord.x);
	first = sqrt(first*first);
	float second = float(target.y-coord.y);
	second = sqrt(second*second);
	return -1*(first+second);
}
//-----------------------------------------------------------------------------------------------
//Generate actions to move towards a target square
//-----------------------------------------------------------------------------------------------
bool MyBot::FindBestPath(square & Target, ActionQueue & Q)
{
	//WANT: path with highest value
	
	//NODE VALUE:
	//-1: Square is seen
	//+: Square isn't seen and has probability
	//0: Square doesn't have probability

	int SearchDepth = 0;
	const int MaxDepth = 10;
	const int ValScale = 4;

	//Start node
	NodeStruct start;
	start.node = Pos;
	start.costSoFar = 0;
	start.edgeFromParent.directionToChild = Orientation;
	start.estimatedTotalCost = CreateHeuristic(start.node,Target);

	//Create open/closed lists
	List OpenList;
	List ClosedList;

	//add start tile to the open list
	OpenList.lst.push_back(start);

	//While there are nodes to process
	NodeStruct currentNode;
	while(!OpenList.lst.empty())
	{
		//Get node from open list with highest value
		currentNode = OpenList.GetStructWithHighestValue();

		//if it is the goal, break
		if((currentNode.node == Target)||(SearchDepth >= MaxDepth))
			break;

		//create the edges from the node
		SearchDepth++;
		deque<edge> edgesToProcess;
		edgesToProcess = CreateOutgoingEdges(currentNode, edgesToProcess);

		//loop through each edge in container
		for(unsigned int i = 0; i < edgesToProcess.size(); i++)
		{
			//get the child node and it's value
			square childNode = edgesToProcess[i].child;
			float childValue = GetSquareValue(childNode);
			float totalCost = currentNode.costSoFar + (childValue*ValScale) + edgesToProcess[i].edgeCost;
			float childHeuristic;
			NodeStruct childStruct;

			//If child tile has already been processed
			if(ClosedList.Contains(childNode)) 
			{
				childStruct = ClosedList.Find(childNode); //Find the node
				if(childStruct.costSoFar >= totalCost) //If stored values smaller, skip to next edge
					continue;
				ClosedList.Remove(childStruct); //Remove it from the closed list
				childHeuristic = childStruct.estimatedTotalCost-childStruct.costSoFar;

			}//If child tile is queued to be processed
			else if(OpenList.Contains(childNode))
			{
				childStruct = OpenList.Find(childNode); //Find the node
				if(childStruct.costSoFar >= totalCost) //If stored values smaller, skip to next edge
					continue;
				OpenList.Remove(childStruct); //Remove it from the open list
				childHeuristic = childStruct.estimatedTotalCost-childStruct.costSoFar;

			}//If child tile has not been visisted create a new tile struct
			else			
			{
				childStruct.node = childNode;
				childHeuristic = CreateHeuristic(childStruct.node,Target);
			}
			
			//update values
			childStruct.edgeFromParent = edgesToProcess[i];
			childStruct.costSoFar = totalCost;
			childStruct.estimatedTotalCost = totalCost + childHeuristic;

			//add/re-add it to the open list
			OpenList.lst.push_back(childStruct);
		}
		//current node edges finished; add current node to the closed list and remove from open list
		OpenList.Remove(currentNode);
		ClosedList.lst.push_back(currentNode);
	}
	//FINISHED PROCESSING

	//If search depth reached
	bool SearchDepthReached = false;
	if(currentNode.node != Target)
	{
		//find the node with the largest value and set as target
		NodeStruct currentNode = ClosedList.GetStructWithLowestValue();
		Target = currentNode.node;
		SearchDepthReached = true;
	}

	//while not at start tile
	deque<NodeStruct> Path;
	while(currentNode.node != Pos)
	{
		Path.push_front(currentNode);
		square parent = currentNode.edgeFromParent.parent;
		currentNode = ClosedList.Find(parent); //look at next tile up
	}
	
	int newOrientation = Orientation;
	for(unsigned int i = 0; i < Path.size(); i++)
	{
		 //new orientation once travelling through edge
		int nextOrientation = Path[i].edgeFromParent.directionToChild;

		//Generate required action
		if(nextOrientation == oUP)
		{
			if(newOrientation == oUP)
			{
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oDOWN)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oRIGHT)
			{
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oLEFT)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
		}
		else if(nextOrientation == oRIGHT)
		{
			if(newOrientation == oUP)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oDOWN)
			{
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oRIGHT)
			{
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oLEFT)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
		}
		else if(nextOrientation == oLEFT)
		{
			if(newOrientation == oUP)
			{
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oDOWN)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oRIGHT)
			{
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oLEFT)
			{
				Q.push_back(act(STEP));
			}
		}
		else if(nextOrientation == oDOWN)
		{
			if(newOrientation == oUP)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oDOWN)
			{
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oRIGHT)
			{
				Q.push_back(act(TURNRIGHT));
				Q.push_back(act(STEP));
			}
			else if(newOrientation == oLEFT)
			{
				Q.push_back(act(TURNLEFT));
				Q.push_back(act(STEP));
			}
		}		
		newOrientation = nextOrientation;
	}

	if(SearchDepthReached)
		return false;
	else
		return true;
}
