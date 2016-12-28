// * ************************************************************************ //
// * * ShroomWorld source                                                 * * //
// * *   : defines ShroomWorld implementation                             * * //
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
// * * ShroomWorld.cpp                                                    * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <time.h>

#include "ShroomWorld.h"
#include "UtilityFunc.h"

ShroomWorld::ShroomWorld()
	: m_xBoard(),
	  m_pParams(NULL),
	  m_pGUIFrame(NULL),
	  m_pGUIBoard(NULL),
	  m_pGUIState(NULL),
	  m_pGUIStats(NULL),
	  m_pHunterControl(NULL),
	  m_pHunterObject(NULL),
	  m_eGameState(PAUSED),
	  m_uTurnCount(0),
	  m_uGameCount(0),
	  m_uFogCleared(0)
{
	m_pHunterObject = new HunterState();
}

ShroomWorld::~ShroomWorld()
{
	SAFE_DELETE(m_pGUIFrame);
	SAFE_DELETE(m_pGUIBoard);
	SAFE_DELETE(m_pGUIStats);
	SAFE_DELETE(m_pGUIState);
	SAFE_DELETE(m_pHunterObject);
	SAFE_DELETE(m_pHunterControl);
}


void ShroomWorld::InitGame()
{
	LARGE_INTEGER __i64Count;
	QueryPerformanceCounter(&__i64Count);
	unsigned __int32 t = __i64Count.LowPart;
	LOG(SAVE) << std::setw(20) << std::left << "SEED" << ": " << t << std::endl;
	//srand( t );

	static int counter = 0;
	srand( counter );
	counter++;
	//TESTING

	m_xBoard.resize(m_pParams->world_rows,m_pParams->world_cols);
	InitBoard();

	COORD cWorldTopLeft,
		  cWorldBottomRight,
		  pos; 
	cWorldTopLeft.X = cWorldTopLeft.Y = 1;
	cWorldBottomRight.X = m_pParams->world_cols-2;
	cWorldBottomRight.Y = m_pParams->world_rows-2;

	int count_sum = 0;
	for (int i = 0; i < m_pParams->shroom_types; ++i)
		count_sum += m_pParams->shroom_count[i];

	// generate SHROOM_NUM shrooms given the distribution in pParams
	for (int i = 0; i < m_pParams->shroom_num; ++i)
	{
		// roulette wheel sampling of the shroom type to create
		//  - generate a random integer in [0,count_sum-1]
		int c = rand() % (count_sum);
		int j=0, sum=0;

		// - spin the wheel
		for (j = 0; j < m_pParams->shroom_types; ++j)
		{
			sum+=m_pParams->shroom_count[j];
			if (sum > c)
				break;
		}

		// generate a random coordinate
		pos = RandomCoordinate(cWorldTopLeft,cWorldBottomRight);
		while (m_xBoard(pos).type != EMPTY)
		{
			pos = RandomCoordinate(cWorldTopLeft,cWorldBottomRight);
		};
		// place a shroom at this location
	

		m_xBoard(pos).type = SHROOM;
		m_xBoard(pos).visible = false;
		m_xBoard(pos).value = m_pParams->shroom_value[j];
		//LOG(GAME) << "Shroom placed at (" 
		//		  << pos.X << "," 
		//		  << pos.Y << ") : value (" 
		//		  << m_pParams->shroom_value[j] 
		//		  << ")" << std::endl;
	}

	// place the Hunter
	if (m_pHunterObject)
	{
		m_pHunterObject->SetFacing( (MAP_DIRECTION)( rand() % NUM_DIRECTIONS ) );
		pos = RandomCoordinate(cWorldTopLeft,cWorldBottomRight);
		while (m_xBoard(pos).type != EMPTY)
		{
			pos = RandomCoordinate(cWorldTopLeft,cWorldBottomRight);
		};

		//TESTING
		//pos.X = 1;
		//pos.Y = 20;
		//pos.Y = 10;
		//pos.X =33;
		//m_pHunterObject->SetFacing( (MAP_DIRECTION)(0));

//WORLD_ROWS         : 22
//WORLD_COLS         : 47
		m_pHunterObject->SetPosition(pos);
		m_pHunterObject->SetHealth(m_pParams->hunter_health);
		m_xBoard(pos).type = HUNTER;
		m_xBoard(pos).visible = true;
//		LOG(GAME) << "Hunter placed at (" << pos.X << "," << pos.Y << ") : value (" << m_pParams->hunter_health << ")" << std::endl;	

		m_pHunterObject->SetCost(STEP,m_pParams->hunter_cost_step);
		m_pHunterObject->SetCost(TURNLEFT,m_pParams->hunter_cost_turn);
		m_pHunterObject->SetCost(TURNRIGHT,m_pParams->hunter_cost_turn);
		m_pHunterObject->SetCost(LOOK,m_pParams->hunter_cost_look);

		//LOG(GAME) << "Hunter action costs set - ";
		//LOG(GAME) << "STEP (" << m_pHunterObject->GetCost(STEP) << "), ";
		//LOG(GAME) << "TURN (" << m_pHunterObject->GetCost(TURNLEFT) << "), ";
		//LOG(GAME) << "LOOK (" << m_pHunterObject->GetCost(LOOK) << ")" << std::endl;

	}

	// place the magic shroom - make sure its not too close to Hunter

	//WORD wIts = 0, wMaxIts = 1000;
	bool bFound = false;
	while (!bFound)
	{
		pos = RandomCoordinate(cWorldTopLeft,cWorldBottomRight);
		
		if ((m_xBoard(pos).type == EMPTY) && (distance(pos,m_pHunterObject->GetPosition()) > 5))
			bFound = true;
	};

	//TESTING
	//pos.Y = 0;
	//pos.X = 0;

	m_xBoard(pos).type = MAGICSHROOM;
	m_xBoard(pos).value = m_pParams->shroom_magic_value;
	m_xBoard(pos).visible = false;
//	LOG(GAME) << "MAGICSHROOM place at (" << pos.X << "," << pos.Y << ") : value (" << m_pParams->shroom_magic_value << ")" << std::endl;

}

void ShroomWorld::InitBoard()
{
	const unsigned int ROWS = m_xBoard.rows();
	const unsigned int COLS = m_xBoard.cols();
	
	for (unsigned int c = 0; c < COLS; c++)
		for (unsigned int r = 0; r < ROWS ; r++)
		{
			if (r == 0 || r == (ROWS-1) || c == 0 || c == (COLS-1))
			{
				m_xBoard(r,c).type = WALL;
				m_xBoard(r,c).visible = true;
			}
			else
			{
				m_xBoard(r,c).type = EMPTY;
				m_xBoard(r,c).visible = false;
			}
		}

}


void ShroomWorld::InitGUI(ShroomStats &rStats)
{
	// set up the GUI
/*
     (r0,c0)
		 ____________________________________________
		|     |                     |                |
		|     dr                    |                |
		|     |                     |                |
		|-dc1-+------- board_cols --+------dc2-------|
		|     |                     |                |
		|     |   ^  BOARD_PANEL    |  STATE_PANEL   |
		|     |   |                 |                |
		|     |board_rows           |  |             |
		|     |   |                 |sc|             |
		|     |   v                 |  |             |
		|     |                     |                |
		|-----+---------------------+----------------|
		|     |                     |                |
		|     dr                    |                |
		|_____|_____________________|________________|



*/
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut != INVALID_HANDLE_VALUE)
	{
		ClearConsole(hStdOut);
		SetConsoleTitle(TEXT("Shroom Game"));
		CONSOLE_CURSOR_INFO	ciCursor = {1, FALSE};
		SetConsoleCursorInfo(hStdOut,&ciCursor);
		SMALL_RECT srWin;

		SHORT r0 = 1, c0 = 1, dr = 2, dc1 = 3, dc2 = 30, sc = 5;
		
		SHORT sWinRows = 2*r0 + 2*dr + m_xBoard.rows();
		SHORT sWinCols = 2*c0 + dc1 + dc2 + m_xBoard.cols();

		COORD2 pos0(r0,c0),
			   pos1(r0+dr,c0+dc1),
			   pos2(r0+dr,dc1 + m_xBoard.cols() + sc),
			   pos3(r0+dr + m_xBoard.rows()/3,dc1 + m_xBoard.cols() + sc);

		srWin.Left = 0;
		srWin.Top = 0;
		srWin.Bottom = srWin.Top + sWinRows - 1;
		srWin.Right = srWin.Left + sWinCols - 1;
		SetConsoleWindowInfo(hStdOut,true,&srWin);
		COORD cWinSize = {sWinCols, sWinRows};
		SetConsoleScreenBufferSize(hStdOut,cWinSize);

		typedef Adaptor1<ShroomWorld,GUIBUFFER&,void>	GUIFUNCTOR;
		// Create a panel for the GUI frame
		SAFE_DELETE(m_pGUIFrame);
		m_pGUIFrame = new GUIPanel(hStdOut,pos0,sWinRows-2*r0,sWinCols-2*c0);
		m_pGUIFrame->SetCallback(new GUIFUNCTOR(this,&ShroomWorld::DrawFrame));

		// force the frame to draw
		m_pGUIFrame->notify();

		// Create a Panel for the board view
		SAFE_DELETE(m_pGUIBoard);
		m_pGUIBoard = new GUIPanel(hStdOut,pos1,m_xBoard.rows(),m_xBoard.cols());
		m_pGUIBoard->SetCallback(new GUIFUNCTOR(this,&ShroomWorld::DrawBoard));

		// Create a Panel for the hunter state view
		typedef Adaptor1<HunterState,GUIBUFFER&,void>	GUISTATEFUNCTOR;
		SAFE_DELETE(m_pGUIState);
		m_pGUIState = new GUIPanel(hStdOut,pos2,5,20);
		m_pGUIState->SetCallback(new GUISTATEFUNCTOR(m_pHunterObject,&HunterState::Draw));
		m_pHunterObject->attach(m_pGUIState);

		m_pGUIState->notify();

		// Create a Panel for the game stats view
		typedef Adaptor1<ShroomStats,GUIBUFFER&,void>	  GUISTATFUNCTOR;
		SAFE_DELETE(m_pGUIStats);
		m_pGUIStats = new GUIPanel(hStdOut,pos3,5,25);
		m_pGUIStats->SetCallback(new GUISTATFUNCTOR(&rStats,&ShroomStats::Draw));
		rStats.attach(m_pGUIStats);

		m_pGUIStats->notify();
	}
}


void ShroomWorld::OnReset(CONFIG_PARAMS *pParams, ShroomStats &rStats, WORD wGameCount)
{
	m_uTurnCount = 0;
	m_uGameCount = wGameCount;
	m_pParams = pParams;

	InitBoard();
	InitGame();
	InitGUI(rStats);

	LOG(SAVE) << *m_pParams;

}


bool ShroomWorld::Connect(ENTITY ent, LPCONTROLLER pBot)
{

	try
	{
		if (ent == HUNTER)
		{
			m_pHunterControl = new Hunter(this,m_pHunterObject);
			m_pHunterControl->Attach(pBot);
			return true;
		}
	}
	catch(std::bad_alloc &ba)
	{
		LOG(ERR) << ba.what() << std::endl;
		return false;
	}

	return false;
}


void ShroomWorld::DrawBoard(GUIBUFFER& rBuf)
{
	// if the board and the buffer have different sizes, we cannot draw
	if (m_xBoard.length() != rBuf.length() )
		return;

	// otherwise, iterate over board and set buffer characters as appropriate

	GAMEBOARD::const_iterator	boardIter = m_xBoard.begin();
	GUIBUFFER::iterator			bufferIter = rBuf.begin();

	for (;
		boardIter != m_xBoard.end(),
		bufferIter != rBuf.end();
		++boardIter,++bufferIter)
	{
		if (! boardIter->visible)
		{
			bufferIter->Char.AsciiChar = (char)TOKEN[FOG];
			bufferIter->Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		}
		else
		{
			bufferIter->Char.AsciiChar = (char)TOKEN[boardIter->type];

			switch (boardIter->type)
			{
				case WALL:
					bufferIter->Attributes = FOREGROUND_WHITE;
					break;
				case EMPTY:
					bufferIter->Attributes = 0;
					break;
				case SHROOM:
					bufferIter->Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
					break;
				case MAGICSHROOM:
					bufferIter->Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
					break;
				case HUNTER:
					bufferIter->Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
			};
		}
	}
	// deal with the Hunter's nose!
	COORD p = GetAdjacentCell(m_pHunterObject->GetPosition(),m_pHunterObject->GetFacing());
	if ( m_xBoard(p).type == EMPTY)
	{
		rBuf(p).Char.AsciiChar = NOSE_TOKEN[m_pHunterObject->GetFacing()];
		rBuf(p).Attributes = FOREGROUND_RED;
	}

}

void ShroomWorld::DrawFrame(GUIBUFFER& rBuf)
{
	if (rBuf.length() == 0)
		return;

	COORD2 size = rBuf.size();
	// set corners
	rBuf(0,0).Char.AsciiChar					= (char)201; // top-left
	rBuf(0,size.col-1).Char.AsciiChar			= (char)187; // top-right
	rBuf(size.row-1,0).Char.AsciiChar			= (char)200; // bottom-left
	rBuf(size.row-1,size.col-1).Char.AsciiChar	= (char)188; // bottom-right
	rBuf(0,0).Attributes					= FOREGROUND_WHITE;
	rBuf(0,size.col-1).Attributes			= FOREGROUND_WHITE;
	rBuf(size.row-1,0).Attributes			= FOREGROUND_WHITE;
	rBuf(size.row-1,size.col-1).Attributes	= FOREGROUND_WHITE;

	// fill in top and bottom border
	for (SHORT c = 1; c < size.col-1; ++c)
	{
		rBuf(0,c).Char.AsciiChar			= (char)205; // top row
		rBuf(0,c).Attributes				= FOREGROUND_WHITE;
		rBuf(size.row-1,c).Char.AsciiChar	= (char)205; // bottom row
		rBuf(size.row-1,c).Attributes		= FOREGROUND_WHITE;
	}
	// fill in left and right border
	for (SHORT r = 1; r < size.row-1; ++r)
	{
		rBuf(r,0).Char.AsciiChar			= (char)186; // top row
		rBuf(r,size.col-1).Char.AsciiChar	= (char)186; // bottom row
		rBuf(r,0).Attributes				= FOREGROUND_WHITE;
		rBuf(r,size.col-1).Attributes		= FOREGROUND_WHITE;
	}
}





GAMEBOARD& ShroomWorld::GetBoard()
{
	return m_xBoard;
}


MAP_DIRECTION ShroomWorld::RelativeToAbsoluteDirection(MAP_DIRECTION const& m, REL_DIRECTION const& r)
{
	return (MAP_DIRECTION)((m + r) % NUM_DIRECTIONS);
}


COORD ShroomWorld::GetAdjacentCell(COORD const &p, MAP_DIRECTION const &m)
{
	COORD dp;
	switch (m)
	{
	case NORTH:
		dp.X = 0; dp.Y = -1;
		break;
	case EAST:
		dp.X = 1; dp.Y = 0;
		break;
	case SOUTH:
		dp.X = 0; dp.Y = 1;
		break;
	case WEST:
		dp.X = -1; dp.Y = 0;
		break;
	};
	return p + dp;
}


WORD ShroomWorld::OnPlay()
{
	LOG(SAVE) << "BEGIN: ";
	m_pHunterControl->OnStart(m_pParams);
	m_uFogCleared = 1; // Hunter always starts with his square visible

	m_eGameState = RUNNING;

	if (m_pGUIBoard)
		m_pGUIBoard->notify();

	do
	{
		// Make percept
		PERCEPT p;
		p.health = m_pHunterObject->GetHealth();
		COORD c = m_pHunterObject->GetPosition();
		m_xBoard(c).visible = true;
		for (unsigned int d = 0; d < NUM_DIRECTIONS; ++d)
		{
			MAP_DIRECTION m = RelativeToAbsoluteDirection(m_pHunterObject->GetFacing(),REL_DIRECTION(d));
			COORD adj = GetAdjacentCell(m_pHunterObject->GetPosition(), m);
			// : do free look - comment out to require LOOK action
			if (! m_xBoard(adj).visible)
			{
				m_xBoard(adj).visible = true;
				m_uFogCleared++;
			}
			p.view[d] = m_xBoard(adj).type;
			p.fog_cleared = m_uFogCleared;
			
			// : uncomment if requiring LOOK action
			//if ( m_xBoard(adj).visible == true )
			//	p.view[d] = m_xBoard(adj).type;
			//else
			//	p.view[d] = FOG;			
		}
		// Check control input
		m_pHunterControl->OnTick(p);

		m_uTurnCount++;

		// test for game over!
		if (m_pHunterObject->GetHealth() <= 0)
		{
			LOG(SAVE) << ": END (LOSS)" << std::endl;
			m_eGameState = LOSE;
		}

		// Update GUI
		if (m_pGUIBoard)
			m_pGUIBoard->notify();
//		OnDisplay();

	} while (m_eGameState == RUNNING);

	m_pHunterControl->OnStop();
	if (m_eGameState == WIN)
		return 1;
	else
		return 0;
}


void ShroomWorld::OnStop()
{
	if (m_eGameState == RUNNING)
		m_eGameState = PAUSED;
}

void ShroomWorld::OnWin()
{
	m_eGameState = WIN;

	LOG(SAVE) << ": END (WIN)" << std::endl;
	OnStop();
}


void ShroomWorld::UpdateStats(ShroomStats &rStats)
{
	float fClear = (float)m_uFogCleared / (float)((m_pParams->world_rows - 2)*(m_pParams->world_cols - 2));
	int iDeficit = m_pParams->hunter_health - m_pHunterObject->GetHealth();
	if (m_eGameState == WIN ) // then magic shroom was eaten
		iDeficit +=  m_pParams->shroom_magic_value;

	float fEfficiency = (float)iDeficit /(float)m_uTurnCount;
	rStats.IncrPlayed();
	if (m_eGameState == WIN)
		rStats.IncrWon();

	rStats.UpdatePercentClear(fClear);
	rStats.UpdateEfficiency(fEfficiency);
}

// ************************************************************************** //

void ShroomStats::UpdatePercentClear(FLOAT fVal)
{
	// should only be called AFTER IncrPlayed()
	m_fPercentClear = (m_fPercentClear * (m_wPlayed - 1) + fVal) / (FLOAT)m_wPlayed;
	if (m_pObs)
		m_pObs->notify();
}

void ShroomStats::UpdateEfficiency(FLOAT fVal)
{
	m_fEfficiency = (m_fEfficiency * (m_wPlayed - 1) + fVal) / (FLOAT)m_wPlayed;
	if (m_pObs)
		m_pObs->notify();
}


void ShroomStats::IncrPlayed()
{
	m_wPlayed++;
	if (m_pObs)
		m_pObs->notify();
}

void ShroomStats::IncrWon()
{
	m_wWon++;
	if (m_pObs)
		m_pObs->notify();
}

WORD const& ShroomStats::Played() const
{
	return m_wPlayed;
}

WORD const& ShroomStats::Won() const
{
	return m_wWon;
}

FLOAT const& ShroomStats::Cleared() const
{
	return m_fPercentClear;
}

FLOAT const& ShroomStats::Efficiency() const
{
	return m_fEfficiency;
}

void ShroomStats::attach(Observer *pObs)
{
	m_pObs = pObs;
}

void ShroomStats::Draw(GUIBUFFER &rBuf)
{
	SHORT rows = rBuf.size().row;
	SHORT cols = rBuf.size().col;

	COORD pos = {0, 0};


	char szWords[5][25] = {
		"GAME STATISTICS",
		"PLAYED     : ",
		"WIN%       : ",
		"CLEAR%     : ",
		"EFFICIENCY : "
	};
	WORD wWinPerc;
	if (m_wPlayed == 0)
		wWinPerc = m_wWon;
	else
		wWinPerc = (WORD)(100.0f * (float)m_wWon / (float)(m_wPlayed));
	WORD wClrPerc = (WORD)(100.0f * m_fPercentClear);

	sprintf_s(szWords[1]+12,10*sizeof(char),"%4u",m_wPlayed);
	sprintf_s(szWords[2]+12,10*sizeof(char),"%4u",wWinPerc);
	sprintf_s(szWords[3]+12,10*sizeof(char),"%4u",wClrPerc);
	sprintf_s(szWords[4]+12,10*sizeof(char),"%3.2f",m_fEfficiency);

	for (SHORT r = 0; r < 5; ++r)
	{
		for (SHORT c = 0; c < 25; ++c)
		{
			char ch = szWords[r][c];
			if (ch == '\0')
				break;
			rBuf(r,c).Char.AsciiChar = ch;
			if (r == 0)
				rBuf(r,c).Attributes = COMMON_LVB_UNDERSCORE | FOREGROUND_WHITE;
			else
				rBuf(r,c).Attributes = FOREGROUND_GREY;

		}

	}
}


std::ostream& operator<<(std::ostream& os, ShroomStats& rStats)
{
	os << "," << rStats.m_wPlayed;
	os << "," << std::setprecision(3) << (float)rStats.m_wWon / (float)rStats.m_wPlayed;
	os << "," << std::setprecision(3) << rStats.m_fPercentClear;
	os << "," << std::setprecision(3) << rStats.m_fEfficiency;

	//os << "PLAYED(#): " << rStats.m_wPlayed << std::endl;
	//os << "WIN   (%): " << std::setw(5) << std::right << std::setprecision(3) << (float)rStats.m_wWon / (float)rStats.m_wPlayed << std::endl;
	//os << "CLEAR (%): " << std::setw(5) << std::right << std::setprecision(3) << rStats.m_fPercentClear << std::endl;
	//os << "EFFICIENCY : " << std::setw(5) << std::right << std::setprecision(3) << rStats.m_fEfficiency << std::endl;

	return os;
}


