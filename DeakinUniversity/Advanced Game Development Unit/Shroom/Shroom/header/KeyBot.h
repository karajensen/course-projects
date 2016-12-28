// * ************************************************************************ //
// * * KeyBot Header                                                      * * //
// * *   : defines Keyboard Controller Interface                          * * //
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
// * * KeyBot.h                                                           * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //
#ifndef KEYBOT_H
#define KEYBOT_H

#include "ShroomBot.h"
#include "GameTypes.h"

class KeyBot : public IController
{
	private:
		LPCONTROLLABLE m_pAvatar;

	public:
		KeyBot();
		~KeyBot();

		virtual char const* name() const;
		virtual char const* id() const;

		virtual void Attach(IControllable * const&);

		virtual void OnStart(CONFIG_PARAMS const *pParams);
		virtual void OnTick(const PERCEPT &);
		virtual void OnStop();
};

#endif