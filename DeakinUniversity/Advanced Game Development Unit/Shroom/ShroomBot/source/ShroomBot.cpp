// * ************************************************************************ //
// * * ShroomBot source                                                   * * //
// * *   : defines factory function Implementations                       * * //
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
// * * ShroomBot.cpp                                                      * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "dllincl.h"
#include "MyBot.h"

BOTAPI INT APIENTRY CreateBot(HINSTANCE hDLL, void** ppInst)
{
	if (!*ppInst)
	{
		try
		{
			// Create an instance of the bot and write its
			// memory address into the block pointed to by ppInst
			*ppInst = (void *)new MyBot();
			if (*ppInst)
				return 1; // success
		}
		catch(...)
		{
			// need to handle error logging back to calling instance
			return -1; // memory allocation error
		}
	}
	return 0; // instance already exists
}


BOTAPI VOID APIENTRY DeleteBot(void** ppInst)
{
	SAFE_DELETE(*ppInst);
}

// ************************************************************************** //
