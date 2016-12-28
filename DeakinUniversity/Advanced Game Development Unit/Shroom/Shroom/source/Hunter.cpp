// * ************************************************************************ //
// * * Hunter Source                                                      * * //
// * *   : defines Hunter class implementation                            * * //
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
// * * Hunter.cpp                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //
#include <iostream>
#include "Hunter.h"
#include "UtilityFunc.h"
#include "ShroomWorld.h"

using std::cerr;
using std::endl;

Hunter::Hunter(ShroomWorld *pG, HunterState *pS)
:	m_cPlan(),
	m_bUsingPlan(false),
	m_pWorld(pG),
	m_pAgent(NULL),
	m_pObject(pS)
{
}

Hunter::~Hunter()
{
	while (!m_cPlan.empty())
		m_cPlan.pop();
}

bool Hunter::ProcessAction(const ACTION &a)
{
	switch (a)
	{
		case STEP:
			LOG(SAVE) << "STEP ";
			OnStep();
			break;

		case TURNLEFT:
			LOG(SAVE) << "TURNLEFT ";
			OnTurn(LEFT);
			break;

		case TURNRIGHT:
			LOG(SAVE) << "TURNRIGHT ";
			OnTurn(RIGHT);
			break;

		case LOOK:
			LOG(SAVE) << "LOOK ";
			OnLook();
			break;

		case EAT:
			LOG(SAVE) << "EAT ";
			OnEat();
			break;

		case QUIT:
			LOG(SAVE) << "QUIT\n";
			m_pWorld->OnStop();
			break;

		default:
			LOG(SAVE) << "\nINVALID\n" << endl;
			return false;
			break;
	};

	return true;

}

bool Hunter::QueueAction(const ACTION &a)
{
	if (a == STEP || a == TURNLEFT || a == TURNRIGHT || a == LOOK || a == EAT)
	{
		m_cPlan.push(a);
		return true;
	}
	else
		return false;
}

void Hunter::ProcessQueue()
{
	if (!m_cPlan.empty())
	{
		m_bUsingPlan = true;
	}
	else
		m_bUsingPlan = false;
}

char const* Hunter::name() const
{
	if (m_pAgent)
		return m_pAgent->name();
	else
		return NULL;
}

char const* Hunter::id() const
{
	if (m_pAgent)
		return m_pAgent->id();
	else
		return NULL;
}

void Hunter::OnStart(CONFIG_PARAMS const *pParams)
{
	if (m_pAgent)
		m_pAgent->OnStart(pParams);
}

void Hunter::OnTick(const PERCEPT &p)
{
	if (m_bUsingPlan)
	{
		if (!m_cPlan.empty())
		{
			ProcessAction(m_cPlan.front());
			m_cPlan.pop();
		}
		else
		{
			m_bUsingPlan = false;
			if (m_pAgent)
				m_pAgent->OnTick(p);
		}
	}
	else if (m_pAgent)
		m_pAgent->OnTick(p);
}

void Hunter::OnStop()
{
	if (m_pAgent)
		m_pAgent->OnStop();
}

void Hunter::Attach(HunterState * const &pState)
{
	m_pObject = pState;
}

void Hunter::Attach(ShroomWorld * const &pWorld)
{
	m_pWorld = pWorld;
}

LPCONTROLLER Hunter::Attach(LPCONTROLLER const &pC)
{
	LPCONTROLLER pTmp = m_pAgent;
	m_pAgent = pC;
	if (m_pAgent)
		m_pAgent->Attach(this);
	return pTmp;
}

void Hunter::OnStep()
{
	// test that hunter can move forward
	COORD p = GetForwardCell();
	GAMEBOARD& board = m_pWorld->GetBoard();

	if ( board(p).type == EMPTY )
	{
		// move hunter forward and set currect cell to EMPTY
		board(m_pObject->GetPosition()).type = EMPTY;
		board(p).type = HUNTER;
		m_pObject->SetPosition(p);
		m_pObject->DecreaseHealth(m_pObject->GetCost(STEP));
	}
}

void Hunter::OnTurn(REL_DIRECTION d)
{
	// Map directions: NORTH = 0, EAST, SOUTH, WEST
	unsigned int map_dir = m_pObject->GetFacing();
	switch (d)
	{
	case LEFT: // decrement one place
		map_dir = (map_dir + 3) % NUM_DIRECTIONS;
		break;
	case RIGHT:
		map_dir = (map_dir + 1) % NUM_DIRECTIONS;
		break;
	default:
		LOG(ERR) << "Invalid turn direction computed" << endl;
	};
	m_pObject->SetFacing((MAP_DIRECTION)map_dir);
	m_pObject->DecreaseHealth(m_pObject->GetCost(TURNLEFT));
}

void Hunter::OnLook()
{
	// queue a PERCEPT for the agent
	//GAMEBOARD& board = m_pWorld->GetBoard();
	//for (unsigned int d = 0; d < NUM_DIRECTIONS; ++d)
	//{
	//	MAP_DIRECTION m = m_pWorld->RelativeToAbsoluteDirection(m_pObject->GetFacing(),REL_DIRECTION(d));
	//	COORD adj = m_pWorld->GetAdjacentCell(m_pObject->GetPosition(), m);
	//	board(adj).visible = true;
	//}

	//m_pObject->DecreaseHealth(m_pObject->GetCost(LOOK));
}

void Hunter::OnEat()
{
	// test cell in front of hunter for shroom
	COORD p = GetForwardCell();
	GAMEBOARD& board = m_pWorld->GetBoard();
	TILE& t = board(p);
	if (t.visible)
	{
		if (t.type == SHROOM)
		{
			// east the shroom
			m_pObject->IncreaseHealth(t.value);
			t.value = 0;
			t.type = EMPTY;
		}
	
		else if (t.type == MAGICSHROOM)
		{
			m_pObject->IncreaseHealth(t.value);
			t.value = 0;
			t.type = EMPTY;
			// winner! Award win
			m_pWorld->OnWin();
		}
	}
}

COORD Hunter::GetForwardCell() const
{
	// determine coordinate in front of hunter
	COORD dp, p = m_pObject->GetPosition();
	switch (m_pObject->GetFacing())
	{
	case NORTH:
		dp.X = 0;
		dp.Y = -1;
		break;
	case EAST:
		dp.X = 1;
		dp.Y = 0;
		break;
	case SOUTH:
		dp.X = 0;
		dp.Y = 1;
		break;
	case WEST:
		dp.X = -1;
		dp.Y = 0;
		break;
	default:
		cerr << "Invalid facing: check OnTurn logic!" << endl;
		return p;
	};
	return ( dp + p );
}


HunterState::HunterState(SHORT const &x, SHORT const& y, MAP_DIRECTION const &f, OBJVALUE const &h)
	: m_eFacing(f), m_uHealth(h), m_pObs(0)
{
	m_sPosition.X = x;
	m_sPosition.Y = y;
	memset(m_uCost,0,NUM_ACTIONS*sizeof(unsigned int));

}

void HunterState::DecreaseHealth(OBJVALUE const& dh)
{
	m_uHealth = max(m_uHealth-dh,0);
	if (m_pObs)
		m_pObs->notify();
}

void HunterState::IncreaseHealth(OBJVALUE const& dh)
{
	m_uHealth += dh;
	if (m_pObs)
		m_pObs->notify();
}

void HunterState::SetHealth(OBJVALUE const& h)
{
	m_uHealth = h;
	if (m_pObs)
		m_pObs->notify();
}

void HunterState::SetFacing(MAP_DIRECTION const& d)
{
	m_eFacing = d;
	if (m_pObs)
		m_pObs->notify();
}

void HunterState::SetPosition(COORD const& c)
{
	m_sPosition = c;
	if (m_pObs)
		m_pObs->notify();
}

void HunterState::SetCost(const ACTION &a, const OBJVALUE &v)
{
	m_uCost[a] = v;
}

OBJVALUE const& HunterState::GetHealth() const
{
	return m_uHealth;
}

MAP_DIRECTION const& HunterState::GetFacing() const
{
	return m_eFacing;
}

COORD const& HunterState::GetPosition() const
{
	return m_sPosition;
}

OBJVALUE const& HunterState::GetCost(const ACTION &a) const
{
	return m_uCost[a];
}

void HunterState::attach(Observer *pObs)
{
	m_pObs = pObs;
}

void HunterState::Draw(GUIBUFFER& rBuf)
{

	SHORT rows = rBuf.size().row;
	SHORT cols = rBuf.size().col;

	COORD pos = {0, 0};


	char szWords[4][20] = {
		"HUNTER STATE",
		"Health   :",
		"Position :",
		"Facing   :"
	};
	sprintf_s(szWords[1]+10,8*sizeof(char)," %5d",m_uHealth);
	sprintf_s(szWords[2]+10,8*sizeof(char)," %2d,%2d",m_sPosition.Y,m_sPosition.X);
	switch (m_eFacing)
	{
		case NORTH:
			sprintf_s(szWords[3]+10,7," %s","NORTH");
			break;
		case SOUTH:
			sprintf_s(szWords[3]+10,7," %s","SOUTH");
			break;
		case EAST:
			sprintf_s(szWords[3]+10,7,"  %s","EAST");
			break;
		case WEST:
			sprintf_s(szWords[3]+10,7,"  %s","WEST");
			break;
		default:
			break;
	};

	for (SHORT r = 0; r < 4; ++r)
	{
		for (SHORT c = 0; c < 20; ++c)
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
