// * ************************************************************************ //
// * * GameManager header                                                 * * //
// * *   : defines GameManager interface                                  * * //
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
// * * GameManager.h                                                    * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef GAMEMGR_H
#define GAMEMGR_H

#include <map>
#include "GameTypes.h"
#include "Hunter.h"
#include "BotLoader.h"


typedef std::map<OBJHANDLE, int>	SHROOM_LUT;


class GameManager
{

	private:

		// game board 
		GAMEBOARD	m_xBoard;

		// store index (location) and value of each mushroom
		SHROOM_LUT	m_cShroomLUT;

		// hunter controller
//		HunterController	*m_pHunterPlayer;
		Hunter		m_sHunter;
		BotLoader	m_sLoader;

		// game state (win, lose, running, replay)
		GAMESTATE	m_eGameState;


		// private method to request move
		// private method to process move

		// private method to notify controller of outcome.

		// method to log gameplay during exection
		void InitBoard();
		void DisplayBoard(unsigned int _row, unsigned int _col);

	public:

		GameManager(unsigned int r, unsigned int c);
		~GameManager();

		GAMEBOARD& GetBoard();

		// Event handlers

		bool OnAction(OBJHANDLE const & hObj, ACTION const & aCommand);

		void Play();

		void OnStop();

		void OnDraw();
};

#endif

// ************************************************************************** //
