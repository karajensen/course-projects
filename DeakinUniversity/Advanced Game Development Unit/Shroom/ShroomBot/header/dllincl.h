// * ************************************************************************ //
// * * DLL utility Header                                                 * * //
// * *   : defines standard includes and macros                           * * //
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
// * * dllincl.h                                                          * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef DLLINCLUDE_H
#define DLLINCLUDE_H

#ifdef _WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#ifndef SAFE_DELETE_DEF
	#define SAFE_DELETE_DEF
	template <typename T> VOID SAFE_DELETE(T* &p) { if (p) { delete p; p = NULL; } }
#endif


#endif

// ************************************************************************** //
