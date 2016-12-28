// * ************************************************************************ //
// * * Application source                                                 * * //
// * *   : defines application entry point                                * * //
// * *                                                                    * * //
// * * Disclaimer: This document represents no significant intellectual   * * //
// * * property and my be used freely. The author(s) takes no             * * //
// * * responsibility for any loss or damage arising from the use of      * * //
// * * the computer code contained herein. Where this document is         * * //
// * * submitted as part of an assessment task this header must remain    * * //
// * * intact and the student must make clear indication which parts      * * //
// * * have been added by themselves.                                     * * //
// * *                                                                    * * //
// * ********************************************************************** * //
// * ********************************************************************** * //
// * * main.cpp                                                           * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cstring>

#include "ShroomWorld.h"
#include "BotLoader.h"
#include "KeyBot.h"

using std::cout;
using std::endl;
using std::cerr;
using std::clog;


const unsigned int BUF_SIZE = MAX_PATH;

void Usage()
{
	cout << "USAGE: -<opt> <opt-arg>" << endl;
	cout << "\t" << "b : bot     <filename> [ load bot from DLL file           ]" << endl;
	cout << "\t" << "c : config  <filename> [ load game parameters from file   ]" << endl;
	cout << "\t" << "d : dir     <directory>[ search for bot file in directory ]" << endl;
	cout << "\t" << "l : log     <filename> [ write event log to file          ]" << endl;
	cout << "\t" << "p : path    <pathname> [ search for files below path      ]" << endl;
	cout << "\t" << "s : save               [ save game(s) to file(s)          ]" << endl;
	cout << "\t" << "r : run     <N>        [ batch mode: run N times          ]" << endl;
}


void ParseArgs(int nArgs,
			   char **vArgs,
			   std::string &strPath,
			   std::string &strBot,
			   std::string &strCfg,
			   std::string &strLog,
			   bool &rSave,
			   WORD &rRun)
{
	strPath.clear();
	strBot.clear();
	strCfg.clear();
	strLog.clear();

	strPath.assign(vArgs[0]);
	strPath = strPath.substr(0,strPath.find("bin"));

	std::string strBotPath = "";

	bool bBotFound = false;
	bool bPathFound = false;

	char szBuffer[BUF_SIZE];

	// make a pass over args and look for -p and -d option to change search path root folders
	for (int a = 1; a < nArgs; ++a)
	{
		// p : path    <filename> [ search for files below path    ]
		if ( strcmp( vArgs[a],"-p" ) == 0 || strcmp( vArgs[a],"-path" ) == 0 )
		{
			bPathFound = true;
			strPath.clear();
			strPath.assign(vArgs[++a]);
		}
		else if ( strcmp( vArgs[a],"-d" ) == 0 || strcmp( vArgs[a],"-dir" ) == 0 )
		{
			strBotPath.assign(vArgs[++a]);
		}
	}

	for (int a = 1; a < nArgs; ++a)
	{
		// ignore the -p option now!
		if ( strcmp( vArgs[a],"-p" ) == 0 || strcmp( vArgs[a],"-path" ) == 0 )
		{
			++a;
		}

		// ignore the -d option now!
		else if ( strcmp( vArgs[a],"-d" ) == 0 || strcmp( vArgs[a],"-dir" ) == 0 )
		{
			++a;
		}

		// b : bot     <filename> [ load bot from DLL file         ]
		else if ( strcmp( vArgs[a],"-b" ) == 0 || strcmp( vArgs[a],"-bot" ) == 0 )
		{
			if (strBotPath.empty())
				strBotPath.assign(strPath);

			strBot.assign(vArgs[++a]);
			// check to see if there is a sub-directory prefix of the bot file
			
			size_t split = strBot.find_last_of('\\');
			// if there is, move it into the path string
			if ( split != std::string::npos)
			{
				strBotPath.append(strBot.substr(0,split+1));
				strBot.erase(0,split+1);
			}

			bBotFound = (0 != SearchPath(strBotPath.c_str(),strBot.c_str(),".dll",BUF_SIZE,szBuffer,NULL));
			strBotPath.clear();
			if (bBotFound)
				strBot.assign(szBuffer);
		}

		// c : config  <filename> [ load game parameters from file ]
		else if ( strcmp(vArgs[a],"-c") == 0 || strcmp(vArgs[a],"-config") == 0 )
		{
			strCfg.clear();
			strCfg.assign(strPath);
			// check to see if there is a sub-directory prefix of the bot file
			std::string strCfgFile;
			strCfgFile.assign(vArgs[++a]);
			size_t split = strCfgFile.find_last_of('\\');
			// if there is, move it into the path string
			if ( split != std::string::npos)
			{
				strCfg.append(strCfgFile.substr(0,split+1));
				strCfgFile.erase(0,split+1);
			}


			if (0 != SearchPath(strCfg.c_str(),strCfgFile.c_str(),".txt",BUF_SIZE,szBuffer,NULL))
			{
				strCfg.clear();
				strCfg.assign(szBuffer);
			}
			else
				strCfg.clear();
		}

		// l : log     <filename> [ write event log to file        ]
		else if ( strcmp(vArgs[a],"-l") == 0 || strcmp(vArgs[a],"-log") == 0 )
		{

			strLog.clear();
			strLog.assign(strPath);
			strLog.append(vArgs[++a]);
			size_t pos = strLog.find(".");
			if (pos == std::string::npos)
				strLog.append(".txt");
		}

		// s : save               [ save game(s) to file(s)        ]
		else if (strcmp(vArgs[a],"-s") == 0 || strcmp(vArgs[a],"-save") == 0)
		{
			rSave = true;
		}
		// r : run     <N>        [ batch mode: run N times        ]
		else if (strcmp(vArgs[a],"-r") == 0 || strcmp(vArgs[a],"-repeat") == 0 )
		{
			std::string strRepeat(vArgs[++a]);
			std::stringstream ssRepeat(strRepeat);
			ssRepeat >>	rRun;
		}
		else
		{
			Usage();
			exit(2);
		}

	}
}

void SetDefaultParams(CONFIG_PARAMS * const &pParams)
{
	pParams->shroom_types = 10;
	pParams->resize(pParams->shroom_types);
	pParams->world_rows = 10;
	pParams->world_cols = 10;
	pParams->shroom_num = 10;
	pParams->hunter_health = 100;
	pParams->hunter_cost_step = 1;
	pParams->hunter_cost_turn = 1;
	pParams->hunter_cost_look = 1;
	for (int i = 0; i < pParams->shroom_types; ++i)
	{
		pParams->shroom_value[i] = -50 + 10 * i;
		pParams->shroom_count[i] = 1;
	}
	pParams->shroom_magic_value = 1000;
}

bool LoadConfig(char const* szFilename, CONFIG_PARAMS * const &pParams)
{
	typedef std::pair<std::string, unsigned int>	entry_type;
	typedef std::map<std::string, unsigned int>		map_type;

	enum str_value
	{
		WORLD_ROWS,
		WORLD_COLS,
		SHROOM_TYPES,
		SHROOM_NUM,
		SHROOM_VALUE,
		SHROOM_COUNT,
		SHROOM_MAGIC_VALUE,
		HUNTER_HEALTH,
		HUNTER_COST_STEP,
		HUNTER_COST_TURN,
		HUNTER_COST_LOOK
	};

	static const entry_type str_value_map[] = 
	{
		entry_type("WORLD_ROWS",			WORLD_ROWS			),
		entry_type("WORLD_COLS",			WORLD_COLS			),
		entry_type("SHROOM_TYPES",			SHROOM_TYPES		),
		entry_type("SHROOM_NUM",			SHROOM_NUM			),
		entry_type("SHROOM_VALUE",			SHROOM_VALUE		),
		entry_type("SHROOM_COUNT",			SHROOM_COUNT		),
		entry_type("SHROOM_MAGIC_VALUE",	SHROOM_MAGIC_VALUE	),
		entry_type("HUNTER_HEALTH",			HUNTER_HEALTH		),
		entry_type("HUNTER_COST_STEP",		HUNTER_COST_STEP	),
		entry_type("HUNTER_COST_TURN",		HUNTER_COST_TURN	),
		entry_type("HUNTER_COST_LOOK",		HUNTER_COST_LOOK	)
	};
	static const size_t count( sizeof(str_value_map) / sizeof(*str_value_map) );
	static map_type	strmap(str_value_map, str_value_map + count);

	std::ifstream fsCfgFile(szFilename);

	if ( !fsCfgFile.is_open() )
		return false;

	std::string	 strLine, strVarName, strSep;

	std::istringstream ssInStrm;


	while( !fsCfgFile.eof() )
	{
		// get the next line from the file
		std::getline(fsCfgFile,strLine);
		// clear the stringstream and push the new line into it
		ssInStrm.clear();
		ssInStrm.str(strLine);
		// extract the parameter name and skip whitespaces and separator
		ssInStrm >> strVarName >> std::ws >> strSep >> std::ws;

		switch (strmap[strVarName])
		{
			case SHROOM_TYPES:
				ssInStrm >> pParams->shroom_types;
				pParams->resize(pParams->shroom_types);
				break;

			case SHROOM_NUM:
				ssInStrm >> pParams->shroom_num;
				break;

			case SHROOM_VALUE:
				if (pParams->shroom_value)
					for (int i = 0; i < pParams->shroom_types; ++i)
						ssInStrm >> pParams->shroom_value[i];
				break;

			case SHROOM_COUNT:
				if (pParams->shroom_count)
					for (int i = 0; i < pParams->shroom_types; ++i)
						ssInStrm >> pParams->shroom_count[i];
				break;

			case SHROOM_MAGIC_VALUE:
				ssInStrm >> pParams->shroom_magic_value;
				break;

			case HUNTER_HEALTH:
				ssInStrm >> pParams->hunter_health;
				break;

			case HUNTER_COST_STEP:
				ssInStrm >> pParams->hunter_cost_step;
				break;

			case HUNTER_COST_TURN:
				ssInStrm >> pParams->hunter_cost_turn;
				break;

			case HUNTER_COST_LOOK:
				ssInStrm >> pParams->hunter_cost_look;
				break;

			case WORLD_ROWS:
				ssInStrm >> pParams->world_rows;
				break;

			case WORLD_COLS:
				ssInStrm >> pParams->world_cols;
				break;	

			default:
				return false;
		};

	};
	return true;

}

void WaitForKey(WORD vkCode)
{

	bool bExit = false;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdIn != INVALID_HANDLE_VALUE)
	{
	    INPUT_RECORD irInBuf[128];
		DWORD cNumRead = 0;

		do
		{
			PeekConsoleInput(hStdIn,irInBuf,128,&cNumRead);
	        for (DWORD i = 0; i < cNumRead; i++) 
		    {
				if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == vkCode)
					bExit = true;
			}

		} while (!bExit);
	}
}

int _cdecl main(int argc, char* argv[])
{
	ShroomWorld		game;
	BotLoader		bot(GetModuleHandle(0));
	CONFIG_PARAMS	cCfgParams;
	LPCONTROLLER	pBot = 0;
	WORD			wRunNum = 1, wRepeatCount = 0;
	WORD			wWinCount = 0;
	bool			bSaveEnabled;
	char			buffer[BUF_SIZE] = {0};


	std::string strBotFile = "";
	std::string strCfgFile = "";
	std::string strLogFile = "";
	std::string strFilePath = "";

	/*
	 * b : bot     <filename> [ load bot from DLL file         ]" << endl;
	 * c : config  <filename> [ load game parameters from file ]" << endl;
	 * p : path    <pathname> [ search for files below path    ]" << endl;
	 * l : log     <filename> [ write event log to file        ]" << endl;
	 * s : save               [ save game(s) to file(s)        ]" << endl;
	 * r : run     <N>        [ batch mode: run N times        ]" << endl;
	 */
	ParseArgs(argc,			// number of command line arguments (includes exe name)
			  argv,			// array of command line arguments
			  strFilePath,	// string to hold file path root directory	: -d option
			  strBotFile,	// string to hold bot file name				: -b option
			  strCfgFile,	// string to hold config file name			: -c option
			  strLogFile,	// string to hold log file name				: -l option
			  bSaveEnabled,	// bool to hold save state					: -s option
			  wRunNum);		// unsigned int to hold number of repeats	: -r option



	std::string strErrLog(strFilePath  + "log\\error.txt");

	std::string strStatFile = "";
	std::string strStudentName = "";


	OPEN_LOG(ERR,strErrLog.c_str());
	if (!strLogFile.empty())
		OPEN_LOG(GAME,strLogFile.c_str());

	if (strBotFile.empty())
	{
		pBot = new KeyBot();
	}	
	else
	{
		if ( bot.Load(strBotFile.c_str()) == true )
		{
			pBot = bot.Get();
		}
		else
		{
			LOG(ERR) << "Unable to load " << strBotFile << "- using keyboard control\n";
			pBot = new KeyBot();
		}
	}

	if (pBot)
	{
		strStudentName.append(pBot->name());
		strStudentName.append("_");
		strStudentName.append(pBot->id());
	}
	else
	{
		LOG(ERR) << "No bot connected: aborting\n";
		CLOSE_LOG(ERR);
		return 1;
	}
	strStatFile.append(strFilePath + "log\\" + strStudentName + ".txt");
	OPEN_LOG(STAT,strStatFile.c_str());
	LOG(STAT) << pBot->id() << "," << pBot->name();



	if (strCfgFile.empty())
	{
//		LOG(GAME) << "No config file given - using default parameters" << "\n";
		SetDefaultParams(&cCfgParams);
	}
	else
	{
		if (!LoadConfig(strCfgFile.c_str(),&cCfgParams))
		{
			LOG(ERR) << "Unable to load config parameters from file: " 
					 << strCfgFile
					 << ", Using default parameters\n";
			SetDefaultParams(&cCfgParams);
		}
	}



	if (!game.Connect(HUNTER,pBot))
		LOG(ERR) << "Unable to connect bot to game instance\n";
	else
	{
		// Play the game(s)!
		

		ShroomStats	cStats;
		while (wRepeatCount++ < wRunNum)
		{
			if (bSaveEnabled)
			{
				std::stringstream ssName;
				ssName << strFilePath << "save\\" << strStudentName << "_save_" << wRepeatCount << ".txt";
				OPEN_LOG(SAVE,ssName.str().c_str());
				ssName.flush();
			}

			game.OnReset(&cCfgParams, cStats, wRepeatCount);
			game.OnPlay();
			game.UpdateStats(cStats);

			if (bSaveEnabled)
				CLOSE_LOG(SAVE);

		};
		LOG(STAT) << cStats;
		//LOG(GAME) << "WIN RATIO : " << std::setprecision(3) << (float)cStats.Won() / (float)cStats.Played() << std::endl;
		//LOG(GAME) << "CLEAR RATIO : " << std::setprecision(3) << cStats.Cleared() << std::endl;
	}


	if (strcmp(pBot->name(),"HUMAN")==0)
	{
		WaitForKey(VK_ESCAPE);
		delete pBot;
	}
	else
		bot.Release();



	return 0;
};

// ************************************************************************** //
