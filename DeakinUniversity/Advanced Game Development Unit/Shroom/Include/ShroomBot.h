// * ************************************************************************ //
// * * ShroomBot Interface Header                                         * * //
// * *   : defines export/import interface for ShroomBot DLL              * * //
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
// * * ShroomBot.h                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //
#ifndef SHROOMBOT_H
#define SHROOMBOT_H

// Define SHROOMBOT_EXPORTS symbol when compiling DLL to export contents of this file
#ifdef SHROOMBOT_EXPORTS
	#define BOTAPI	__declspec(dllexport)
#else
	#define BOTAPI	__declspec(dllimport)
#endif

// Currently a non-portable dependency on Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// base message types to be passed between controller and controllable objects
#include "MsgTypes.h"
#include "Config.h"


#ifdef __cplusplus // C++ interface

	// abstract (pure virtual) base classes don't require a declspec specifier

	// forward declaration to deal with circular dependence
	class IControllable;

	// Controller Interface class
	class IController
	{
		public:
			virtual ~IController(){}

			virtual char const* name() const = 0;
			virtual char const* id() const = 0;

			virtual void Attach(IControllable * const&) = 0;

			virtual void OnStart(CONFIG_PARAMS const*) = 0;
			virtual void OnTick(const PERCEPT &) = 0;
			virtual void OnStop() = 0;

		protected:
			IController(){}
			IController(IController const&){}
			IController& operator=(IController const&){}

	};


	// Controllable Interface class
	class IControllable
	{

		public:
			virtual ~IControllable(){}

			virtual bool ProcessAction(const ACTION &) = 0;	// Process a single action this turn
			virtual bool QueueAction(const ACTION &) = 0;		// Queue an action
			virtual void ProcessQueue() = 0;					// Process action queue, one per turn until empty

		protected:
			IControllable(){}
			IControllable(IControllable const &){}
			IControllable& operator=(IControllable const &){}
			
	};

	typedef IController		*LPCONTROLLER;
	typedef IControllable	*LPCONTROLLABLE;


	extern "C"
	{
		// Factory functions
		BOTAPI INT	APIENTRY CreateBot(HINSTANCE, void**);
		BOTAPI VOID APIENTRY DeleteBot(void**);

		// Factory function pointer typedefs
		typedef BOTAPI INT (APIENTRY *FPCREATE)(HINSTANCE,void**);
		typedef BOTAPI VOID (APIENTRY *FPDELETE)(void**);
	}
#endif


#endif

// ************************************************************************** //
