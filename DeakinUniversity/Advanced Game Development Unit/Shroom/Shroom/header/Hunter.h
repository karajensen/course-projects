// * ************************************************************************ //
// * * Hunter header                                                      * * //
// * *   : defines Hunter class interface                                 * * //
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
// * * Hunter.h                                                           * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef HUNTER_H
#define HUNTER_H

#include <queue>
#include <list>

#include "ShroomBot.h"
#include "GameTypes.h"
#include "ShroomLOG.h"
#include "ShroomGUI.h"
#include "Observer.h"


class HunterState : public Observable
{
	private:
		COORD			m_sPosition;
		MAP_DIRECTION	m_eFacing;
		OBJVALUE		m_uHealth,
						m_uCost[NUM_ACTIONS];
		Observer		*m_pObs;


	public:
		HunterState(SHORT const &x = -1, SHORT const& y = -1, MAP_DIRECTION const &f = NORTH, OBJVALUE const &h = 0);

		void DecreaseHealth(OBJVALUE const& dh);
		void IncreaseHealth(OBJVALUE const& dh);

		void SetHealth(OBJVALUE const& h);
		void SetFacing(MAP_DIRECTION const& d);
		void SetPosition(COORD const& c);
		void SetCost(ACTION const& a, OBJVALUE const& v);

		OBJVALUE const& GetHealth() const;
		MAP_DIRECTION const& GetFacing() const;
		COORD const& GetPosition() const;
		OBJVALUE const& GetCost(ACTION const& a) const;

		void Draw(GUIBUFFER&);
		// inherited from Observable interface
		void attach(Observer *);
};

class ShroomWorld;

// The Hunter class implements the IControllable interface
class Hunter : public IControllable, public IController
{
	private:

		ShroomWorld		*m_pWorld;
		HunterState		*m_pObject;
		IController		*m_pAgent;

		// provide for buffering of actions
		std::queue< ACTION, std::list<ACTION> >	m_cPlan;
		bool			m_bUsingPlan;

		void OnStep();
		void OnTurn(REL_DIRECTION d);
		void OnLook();
		void OnEat();

		COORD GetForwardCell() const;

	public:
		explicit Hunter(ShroomWorld *pG = NULL, HunterState *pState = NULL);
		~Hunter();

		// Implement Controllable Interface
		virtual bool ProcessAction(const ACTION &a);	// Process a single action this turn
		virtual bool QueueAction(const ACTION &a);		// Queue an action
		virtual void ProcessQueue();

		virtual void OnStart(CONFIG_PARAMS const *pParams);
		virtual void OnTick(const PERCEPT &p);
		virtual void OnStop();

		virtual char const* name() const;
		virtual char const* id() const;
		virtual void Attach(LPCONTROLLABLE const &pObj){}

		LPCONTROLLER Attach(LPCONTROLLER const &pC);

		void Attach(HunterState * const &pState);
		void Attach(ShroomWorld * const &pWorld);

};






#endif