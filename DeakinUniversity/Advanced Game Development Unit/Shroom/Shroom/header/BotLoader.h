// * ************************************************************************ //
// * * BotLoader header                                                   * * //
// * *   : defines BotLoader interface                                    * * //
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
// * * BotLoader.h                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef BOTLOADER_H
#define BOTLOADER_H

#include "ShroomBot.h"

class BotLoader
{
	private:
		LPCONTROLLER	m_pBot;
		HINSTANCE		m_hInstance;
		HMODULE			m_hModule;

	public:
		explicit BotLoader(HINSTANCE h) : m_hInstance(h), m_hModule(NULL), m_pBot(NULL) {}
		~BotLoader();

		bool			Load(char const* dllname);
		void			Release();
		LPCONTROLLER	Get();

};

#endif

// ************************************************************************** //
