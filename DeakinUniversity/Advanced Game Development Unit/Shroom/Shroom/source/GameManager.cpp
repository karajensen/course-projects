// * ************************************************************************ //
// * * GameManager source                                                 * * //
// * *   : defines GameManager implementation                             * * //
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
// * * GameManager.cpp                                                    * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "GameManager.h"
#include "UtilityFunc.h"

GameManager::GameManager(unsigned int r, unsigned int c)
	: m_xBoard(r,c),
	  m_cShroomLUT(),
	  m_sHunter(*this),
	  m_sLoader(GetModuleHandle(0)),
	  m_eGameState(PAUSED)
{
}

GameManager::~GameManager()
{
}

void GameManager::InitBoard()
{
	const unsigned int ROWS = m_xBoard.rows();
	const unsigned int COLS = m_xBoard.cols();
	
	for (unsigned int c = 0; c < COLS; c++)
		for (unsigned int r = 0; r < ROWS ; r++)
		{
			if (r == 0 || r == (ROWS-1) || c == 0 || c == (COLS-1))
			{
				m_xBoard(r,c).type = WALL;
				m_xBoard(r,c).visible = false;
			}
			else
			{
				m_xBoard(r,c).type = EMPTY;
				m_xBoard(r,c).visible = false;
			}
		}

	// initialise SHROOMs, MAGICSHROOM and HUNTER


}

void GameManager::DisplayBoard(unsigned int _row, unsigned int _col)
{
	ClearConsole();

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	DWORD dNumRead = 0;

	// loop over board cells
	for (unsigned int c = 0; c < m_xBoard.cols(); ++c)
		for (unsigned int r = 0; r < m_xBoard.rows(); ++r)
		{
			// check to see if this cell is visible and
			// set diplay character appropriately
			TILE &t = m_xBoard(r,c);
			char ch;
			if (t.visible)
				ch = (char)TOKEN[t.type];
			else
				ch = (char)TOKEN[FOG];

			// determine position to draw at
			COORD p;
			p.X = c + _col;
			p.Y = r + _row;

			WriteConsoleOutputCharacterA(hStdOut,&ch,1,p,&dNumRead);
		}
}

GAMEBOARD& GameManager::GetBoard()
{
	return m_xBoard;
}

bool GameManager::OnAction(OBJHANDLE const & hObj, ACTION const & aCommand)
{
	// Look up object handle to retrieve game object to pass action to
	// Trivial in this game
	return true;
}

void GameManager::Play()
{
	InitBoard();

	OnDraw();
}

void GameManager::OnStop()
{
}

void GameManager::OnDraw()
{
	DisplayBoard(0,0);
}

// ************************************************************************** //
