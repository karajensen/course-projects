#ifndef MYSTRUCT_H_
#define MYSTRUCT_H_

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <algorithm> 
#include "ShroomBot.h"

using namespace std;

enum {OUTERSPIRAL=0,ZIGZAG,INNERSPIRAL,CLEANUP,PREDICTION,ZIGZAGFINAL};
enum {oUP=0,oRIGHT,oDOWN,oLEFT,oUNKNOWN};
enum {pY=0,nY,pX,nX};
enum {MAP1=0 ,MAP2,MAP3,MAPNONE};

struct square
{
	int x,y;
	square(int X=0,int Y=0):x(X),y(Y){};
	bool operator==(square & b)
	{
		if((b.x == this->x)&&(b.y == this->y)){return true;}
		else {return false;}
	}
	bool operator!=(square & b)
	{
		if((b.x != this->x)||(b.y != this->y)){return true;}
		else{return false;}
	}
};
struct node
{
	square coord;
	int type;
	bool virtualWall;
};
struct act
{
	ACTION action;
	act(){};
	act(ACTION newAction):action(newAction){};
};
struct mushType
{
	float count;
	float value;
	float probability;
	float probxvalue;
};
struct Wall
{
	bool UpWallReached;
	bool LeftWallReached;
	bool RightWallReached;
	bool DownWallReached;
	square topleft;
	square bottomright;
	bool FoundAllWalls;
};
struct MS
{
	int distance;
	int count;
	float probability;
	MS(int Dis,int Count,float Prob):distance(Dis),count(Count),probability(Prob){};
	MS(){};
};
struct MSDfunctor : public std::binary_function<MS,MS,bool>
{
	bool operator()(const MS & a, const MS & b)
	{
		if(a.probability < b.probability)
			return true;
		else
			return false;
	}
};

typedef square vec2D;
typedef deque<act> ActionQueue;
typedef deque<act>::iterator ActionQueueIt;
typedef vector<node> NodeContainer;
typedef vector<square> SquareContainer;
typedef vector<square>::iterator SquareContainerIt;
typedef vector<MS> MagicShroomContainer;
typedef vector<MS>::iterator MagicShroomContainerIt;
typedef vector<mushType> MushroomSpread;
typedef string mapKeyType;
typedef node mapNodeType;
typedef pair<const mapKeyType,mapNodeType> MapPair;
typedef map<mapKeyType,mapNodeType> ShroomMap;

////////////////////////////////////////////////////////////////////////////////////
//PATHFINDING STRUCTS
////////////////////////////////////////////////////////////////////////////////////
struct edge
{
	float edgeCost;
	square parent;
	square child;
	int directionToChild; //new orientation once travelling through edge
};
struct NodeStruct
{
	square node;
	edge edgeFromParent;
	float costSoFar;
	float estimatedTotalCost;
};
struct List
{
	deque<NodeStruct> lst;
	NodeStruct & GetStructWithHighestValue()
	{
		deque<NodeStruct>::iterator largest = lst.begin();
		deque<NodeStruct>::iterator it;
		for (it = lst.begin()+1; it != lst.end(); it++)
		{
			if((*it).estimatedTotalCost > (*largest).estimatedTotalCost)
				largest = it;
		}
		return *largest;
	}
	NodeStruct & GetStructWithLowestValue()
	{
		deque<NodeStruct>::iterator smallest = lst.begin();
		deque<NodeStruct>::iterator it;
		for (it = lst.begin()+1; it != lst.end(); it++)
		{
			if((*it).estimatedTotalCost < (*smallest).estimatedTotalCost)
				smallest = it;
		}
		return *smallest;
	}
	bool Contains(square & coord)
	{
		deque<NodeStruct>::iterator it;
		for (it = lst.begin(); it != lst.end(); it++)
		{
			if((*it).node == coord)
				return true;
		}
		return false;
	}
	NodeStruct & Find(square & coord)
	{
		deque<NodeStruct>::iterator it;
		for (it = lst.begin(); it != lst.end(); it++)
		{
			if((*it).node == coord)
				return (*it);
		}
		return (*it);
	}
	bool Remove(NodeStruct & removeNodeStruct)
	{
		deque<NodeStruct>::iterator it;
		for (it = lst.begin(); it != lst.end(); it++)
		{
			if((*it).node == removeNodeStruct.node)
			{
				lst.erase(it);
				return true;
			}
		}
		return false;
	}
	bool Remove(square & coord)
	{
		deque<NodeStruct>::iterator it;
		for (it = lst.begin(); it != lst.end(); it++)
		{
			if((*it).node == coord)
			{
				lst.erase(it);
				return true;
			}
		}
		return false;
	}
};

#endif