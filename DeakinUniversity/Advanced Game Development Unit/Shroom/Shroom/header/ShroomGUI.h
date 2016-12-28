// * ************************************************************************ //
// * * ShroomGUI header                                                   * * //
// * *   : defines classes for creating console GUI elements based on     * * //
// * *     observer design pattern and functor/adaptor callbacks          * * //
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
// * * ShroomGUI.h                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 26/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //

#ifndef SHROOMGUI_H
#define SHROOMGUI_H

#include <list>

#include "GameTypes.h"
#include "Callback.h"
#include "Observer.h"




// Generic GUI element contains a buffer to store character data, a region
// variable and can be attached to a functor which does the work of translating
// the observed object into the view data
class GUIPanel : public Observer
{

	public:
		typedef BFunctor1<GUIBUFFER&,void> *FPCALLBACK;

		GUIPanel(HANDLE h, SHORT top, SHORT left, SHORT bottom, SHORT right);
		GUIPanel(HANDLE h, COORD2 pos, SHORT rows, SHORT cols);
		~GUIPanel();

		void SetCallback(FPCALLBACK fpFunc);

		// Inherited from Observer interface
		void notify();


	private:
		HANDLE		m_hOut;
		SMALL_RECT	m_srRegion;
		GUIBUFFER	m_ciBuffer;
		FPCALLBACK	m_fpCallback;

		void Draw();

};

#endif