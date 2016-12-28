// * ************************************************************************ //
// * * BotLoader source                                                   * * //
// * *   : defines BotLoader implementation                               * * //
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
// * * BotLoader.cpp                                                      * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "BotLoader.h"

BotLoader::~BotLoader()
{
	if (m_pBot)
		Release();
}

bool BotLoader::Load(char const* dllname)
{
	// Load requested dll - m_hModule NULL on failure
	m_hModule = LoadLibrary(dllname);
	if (m_hModule)
	{
		// Get address of CreateInstance function from library - store in function pointer
		FPCREATE fpBotFactory = 0;
		fpBotFactory = (FPCREATE)GetProcAddress( m_hModule, "CreateBot" );

		// If successful in getting address of CreateInstance, then create bot in dll memory space
		// pass memory location in which to store address value of bot instance
		if (fpBotFactory)
			fpBotFactory(m_hModule,(void **)&m_pBot);

		if ( m_pBot )
			return true;
	}
	return false;

}

void BotLoader::Release()
{
	if (m_hModule)
	{
		if (m_pBot)
		{
			// Get address of DeleteInstance function from library - store in function pointer
			FPDELETE fpGarbage = 0;
			fpGarbage = (FPDELETE)GetProcAddress( m_hModule, "DeleteBot" );

			// delete bot in dll memory space
			fpGarbage( (void **)&m_pBot );
		}
		// Unload the DLL
		FreeLibrary(m_hModule);
	}
}

LPCONTROLLER BotLoader::Get()
{
	return m_pBot;
}

// ************************************************************************** //
