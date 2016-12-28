// * ************************************************************************ //
// * * dllmain source                                                     * * //
// * *   : defines entry point for DLL application                        * * //
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
// * * dllmain.cpp                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "dllincl.h"

// turn off managed code features
#ifdef _MANAGED
	#pragma managed(push, off)
#endif


BOOL APIENTRY DllEntyPoint(HINSTANCE l_hDLL,
						   DWORD l_dwReason,
						   LPVOID l_pvReserved)
{
	switch(l_dwReason)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
	};
	
	return TRUE;
}

#ifdef _MANAGED
	#pragma managed(pop)
#endif

// ************************************************************************** //
