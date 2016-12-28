#include "MyBot.h"

//MSD: MAGIC SHROOM DISTRIBUTION holds distances from hunter starting pos to found magic shrooms and probabilities
//PREDICTION MAP: A container of MSDcoords which hold coordinates on the map and a value based on the MSD
MagicShroomContainer MyBot::MSD;
int MyBot::TotalMSfound = 0;
bool MyBot::SaveMSD = false;
bool MyBot::UseSavedMSD = true;

//-----------------------------------------------------------------------------------------------
//Add the magic shroom to MSD
//-----------------------------------------------------------------------------------------------
void MyBot::SaveMagicShroom(square & coord)
{
	if(SaveMSD)
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
		TotalMSfound++;
	}
}
//-----------------------------------------------------------------------------------------------
//Calculate the magic shroom probabilities from MSD
//-----------------------------------------------------------------------------------------------
void MyBot::MagicShroomProbability()
{
	//probability = count/overall count
	for(unsigned int i = 0; i < MSD.size(); i++)
		MSD[i].probability = (float(MSD[i].count))/(float(TotalMSfound));

	//sort in order of smallest probability to largest
	sort(MSD.begin(), MSD.end(), MSDfunctor());

	//save the index searcher
	SearchIndex = (MSD.size()/2);
	if(SearchIndex < 0)
		SearchIndex = 0;
}
//-----------------------------------------------------------------------------------------------
//Find the distance band for a given square
//-----------------------------------------------------------------------------------------------
int MyBot::FindDistanceBand(square & coord)
{
	int x = coord.x;
	int y = coord.y;
	if(x < 0)
		x = x*-1;
	if(y < 0)
		y = y*-1;
	int band = max(x,y);
	if(band < 0)
		band = band*-1;
	return band;
}
//-----------------------------------------------------------------------------------------------
//Generate a value for the coord
//-----------------------------------------------------------------------------------------------
float MyBot::GetSquareValue(square & coord)
{
	//probability will equal 0 if no entry found
	float probability = 0;

	//Find distance band
	int distance = FindDistanceBand(coord);

	//check if square has been seen or not
	if(NodeExists(coord))
		return -1;

	//Find corresponding entry into MSD container
	for(unsigned int i = 0; i < MSD.size(); i++)
	{
		if(MSD[i].distance == distance)
		{
			probability = MSD[i].probability;
			break;
		}
	}
	//return corresponding probability
	return probability;
}
//-----------------------------------------------------------------------------------------------
//Save the final MSD into a text file
//-----------------------------------------------------------------------------------------------
void MyBot::SaveMSDtoFile()
{
	DynamicPath = false;
	PathChosen = CLEANUP;
	CleanUpDepth = 1;

	if(trialNo >= 999)
	{
		TotalMSfound = TotalMSfound;
		ofstream MSDfile;
		MSDfile.open("MSDfile.txt");
		for(unsigned int i = 0; i < MSD.size();i++)
		{
			MSDfile << MSD[i].distance << " ";
			MSDfile << MSD[i].count << " ";
			MSDfile << MSD[i].probability;
			if(i < (MSD.size()-1))
				MSDfile << endl;
		}
		MSDfile.close();
	}
}
//-----------------------------------------------------------------------------------------------
//Load Magic shroom distribution
//-----------------------------------------------------------------------------------------------
void MyBot::LoadMSD()
{
	if(SaveMSD)
		return;

	if(UseSavedMSD)
	{
		//LOAD MSD FROM HARDCODED VALUES
		if(MapNo == MAP1)
		{
			//MSD.push_back(MS(dis,count,prob)));
			MSD.push_back(MS(4,7,float(0.00700701)));
			MSD.push_back(MS(5,57,float(0.0570571)));
			MSD.push_back(MS(6,67,float(0.0670671)));
			MSD.push_back(MS(7,80,float(0.0800801)));
			MSD.push_back(MS(8,85,float(0.0850851)));
			MSD.push_back(MS(9,82,float(0.0820821)));
			MSD.push_back(MS(10,95,float(0.0950951)));
			MSD.push_back(MS(11,74,float(0.0740741)));
			MSD.push_back(MS(12,80,float(0.0800801)));
			MSD.push_back(MS(13,78,float(0.0780781)));
			MSD.push_back(MS(14,69,float(0.0690691)));
			MSD.push_back(MS(15,53,float(0.0530531)));
			MSD.push_back(MS(16,42,float(0.042042)));
			MSD.push_back(MS(17,43,float(0.043043)));
			MSD.push_back(MS(18,35,float(0.035035)));
			MSD.push_back(MS(19,28,float(0.028028)));
			MSD.push_back(MS(20,22,float(0.022022)));
			MSD.push_back(MS(21,2,float(0.002002)));
		}
		else if(MapNo == MAP2)
		{
			MSD.push_back(MS(4,4,float(0.004004)));
			MSD.push_back(MS(5,43,float(0.043043)));
			MSD.push_back(MS(6,52,float(0.0520521)));
			MSD.push_back(MS(7,53,float(0.0530531)));
			MSD.push_back(MS(8,71,float(0.0710711)));
			MSD.push_back(MS(9,57,float(0.0570571)));
			MSD.push_back(MS(10,53,float(0.0530531)));
			MSD.push_back(MS(11,55,float(0.0550551)));
			MSD.push_back(MS(12,61,float(0.0610611)));
			MSD.push_back(MS(13,60,float(0.0600601)));
			MSD.push_back(MS(14,55,float(0.0550551)));
			MSD.push_back(MS(15,45,float(0.045045)));
			MSD.push_back(MS(16,57,float(0.0570571)));
			MSD.push_back(MS(17,41,float(0.041041)));
			MSD.push_back(MS(18,46,float(0.046046)));
			MSD.push_back(MS(19,40,float(0.04004)));
			MSD.push_back(MS(20,32,float(0.032032)));
			MSD.push_back(MS(21,46,float(0.046046)));
			MSD.push_back(MS(22,20,float(0.02002)));
			MSD.push_back(MS(23,20,float(0.02002)));
			MSD.push_back(MS(24,15,float(0.015015)));
			MSD.push_back(MS(25,22,float(0.022022)));
			MSD.push_back(MS(26,18,float(0.015015)));
			MSD.push_back(MS(27,7,float(0.00700701)));
			MSD.push_back(MS(28,12,float(0.012012)));
			MSD.push_back(MS(29,7,float(0.00700701)));
			MSD.push_back(MS(30,7,float(0.00700701)));
		}
		else if(MapNo == MAP3)
		{
			MSD.push_back(MS(4,2,float(0.002002)));
			MSD.push_back(MS(5,41,float(0.041041)));
			MSD.push_back(MS(6,40,float(0.04004)));
			MSD.push_back(MS(7,47,float(0.047047)));
			MSD.push_back(MS(8,51,float(0.0510511)));
			MSD.push_back(MS(9,56,float(0.0560561)));
			MSD.push_back(MS(10,41,float(0.041041)));
			MSD.push_back(MS(11,57,float(0.0570571)));
			MSD.push_back(MS(12,55,float(0.0550551)));
			MSD.push_back(MS(13,52,float(0.0520521)));
			MSD.push_back(MS(14,38,float(0.038038)));
			MSD.push_back(MS(15,44,float(0.044044)));
			MSD.push_back(MS(16,34,float(0.034034)));
			MSD.push_back(MS(17,38,float(0.038038)));
			MSD.push_back(MS(18,33,float(0.033033)));
			MSD.push_back(MS(19,33,float(0.033033)));
			MSD.push_back(MS(20,32,float(0.032032)));
			MSD.push_back(MS(21,15,float(0.015015)));
			MSD.push_back(MS(22,19,float(0.019019)));
			MSD.push_back(MS(23,28,float(0.028028)));
			MSD.push_back(MS(24,25,float(0.025025)));
			MSD.push_back(MS(25,20,float(0.02002)));
			MSD.push_back(MS(26,21,float(0.021021)));
			MSD.push_back(MS(27,20,float(0.02002)));
			MSD.push_back(MS(28,17,float(0.017017)));
			MSD.push_back(MS(29,18,float(0.018018)));
			MSD.push_back(MS(30,14,float(0.014014)));
			MSD.push_back(MS(31,15,float(0.015015)));
			MSD.push_back(MS(32,10,float(0.01001)));
			MSD.push_back(MS(33,21,float(0.021021)));
			MSD.push_back(MS(34,8,float(0.00800801)));
			MSD.push_back(MS(35,13,float(0.013013)));
			MSD.push_back(MS(36,7,float(0.00700701)));
			MSD.push_back(MS(37,8,float(0.00800801)));
			MSD.push_back(MS(38,14,float(0.014014)));
			MSD.push_back(MS(39,2,float(0.002002)));
			MSD.push_back(MS(40,4,float(0.004004)));
			MSD.push_back(MS(41,1,float(0.001001)));
			MSD.push_back(MS(42,2,float(0.002002)));
			MSD.push_back(MS(43,3,float(0.003003)));
		}
	}
	TotalMSfound = 0;
	for(unsigned int i = 0; i < MSD.size(); i++)
		TotalMSfound += MSD[i].count;
	TotalMSfound = TotalMSfound;
	UseSavedMSD = false;
	MagicShroomProbability();
}


//open file and read into MSD
/*
ifstream MSDfile("MSDfile.txt");
if(MSDfile.is_open())
{
	int dis;
	int count;
	float prob;
	int counter = 0;
	while(!MSDfile.eof())
	{
		MSDfile >> dis;
		MSDfile.get();
		MSDfile >> count;
		MSDfile.get();
		MSDfile >> prob;
		MSDfile.get();
		MS temp(dis,count,prob);
		MSD.push_back(temp);
		counter+=count;
	}
	TotalMSfound = counter;
}
MSDfile.close();
*/