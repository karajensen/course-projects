// * ************************************************************************ //
// * * ShroomWorld header                                                 * * //
// * *   : defines ShroomWorld interface                                  * * //
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
// * * ShroomWorld.h                                                    * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef SHROOMWORLD_H
#define SHROOMWORLD_H

#include <map>
#include "GameTypes.h"
#include "Hunter.h"
#include "BotLoader.h"
#include "ShroomGUI.h"
#include "ShroomLOG.h"
#include "Observer.h"


class ShroomStats : public Observable
{
	public:
		ShroomStats()
			: m_pObs(0),
			m_wPlayed(0),
			m_wWon(0),
			m_fPercentClear(0.0f),
			m_fEfficiency(0.0f)
		{}

		void UpdatePercentClear(FLOAT);
		void UpdateEfficiency(FLOAT);

		void IncrPlayed();
		void IncrWon();

		WORD const& Played() const;
		WORD const& Won() const;
		FLOAT const& Cleared() const;
		FLOAT const& Efficiency() const;


		void attach(Observer *);

		void Draw(GUIBUFFER&);

		friend std::ostream& operator<<(std::ostream&, ShroomStats&);

	private:
		Observer	*m_pObs;
		WORD	m_wPlayed,
				m_wWon;
		FLOAT	m_fPercentClear,
				m_fEfficiency;

};

class ShroomWorld
{

	private:

		// config parameters
		CONFIG_PARAMS	*m_pParams;

		// game board 
		GAMEBOARD		m_xBoard;

		// GameGUI
		GUIPanel		*m_pGUIFrame,
						*m_pGUIBoard,
						*m_pGUIState,
						*m_pGUIStats;

		// hunter controller
		Hunter			*m_pHunterControl;

		// Hunter state data
		HunterState		*m_pHunterObject;

		// game state (win, lose, running, replay)
		GAMESTATE		m_eGameState;

		WORD			m_uFogCleared,
						m_uTurnCount,
						m_uGameCount;



		void InitGame();
		void InitBoard();
		void InitGUI(ShroomStats&);

		void DrawBoard(GUIBUFFER& rBuf);
		void DrawFrame(GUIBUFFER& rBuf);

		MAP_DIRECTION RelativeToAbsoluteDirection(MAP_DIRECTION const& m, REL_DIRECTION const& r);
		COORD GetAdjacentCell(COORD const &p, MAP_DIRECTION const &m);


	public:

		ShroomWorld();
		~ShroomWorld();

		GAMEBOARD& GetBoard();
		
		void UpdateStats(ShroomStats &rStats);

		bool Connect(ENTITY, LPCONTROLLER);

		void OnReset(CONFIG_PARAMS *pParams, ShroomStats &rStats, WORD wGameCount = 1);
		WORD OnPlay();
		void OnWin();
		void OnStop();
};




#endif

// ************************************************************************** //
