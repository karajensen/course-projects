// * ************************************************************************ //
// * * KeyBot Source                                                      * * //
// * *   : defines Keyboard Controller Implementation                     * * //
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
// * * KeyBot.cpp                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include <iostream>
#include "KeyBot.h"


KeyBot::KeyBot()
{
}

KeyBot::~KeyBot()
{
}

char const* KeyBot::name() const
{
	// Insert student surname here
	return "HUMAN";
}

char const* KeyBot::id() const
{
	// insert student id number here
	return "0";
}

void KeyBot::Attach(LPCONTROLLABLE const& pA)
{
	m_pAvatar = pA;
}

void KeyBot::OnStart(CONFIG_PARAMS const *pParams)
{
	if (!m_pAvatar)
		std::cerr << "KeyBot Error: not connected" << std::endl;
	return;
}

void KeyBot::OnTick(const PERCEPT &)
{
	if (!m_pAvatar)
	{
		std::cerr << "KeyBot Error: not connected" << std::endl;
		return;
	}

	const unsigned int MAX_INPUTS = 128;
	DWORD dNumRead;
	INPUT_RECORD	irBuffer[MAX_INPUTS];
	HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);

	if (hStdInput == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Unable to acquire STD_INPUT_HANDLE for KeyBot" << std::endl;
		return;
	}
	// get input... 
	ReadConsoleInput(hStdInput,irBuffer,MAX_INPUTS,&dNumRead);
	for (DWORD i = 0; i<dNumRead; i++)
	{
		if (irBuffer[i].EventType == KEY_EVENT)
		{
			if (!irBuffer[i].Event.KeyEvent.bKeyDown)
				continue;
			WORD wVKCode = irBuffer[i].Event.KeyEvent.wVirtualKeyCode;
			switch (wVKCode)
			{
				case VK_LEFT:
					m_pAvatar->ProcessAction(TURNLEFT);
					break;
				case VK_RIGHT:
					m_pAvatar->ProcessAction(TURNRIGHT);
					break;
				case VK_UP:
					m_pAvatar->ProcessAction(STEP);
					break;
				case VK_DOWN:
					m_pAvatar->ProcessAction(LOOK);
					break;
				case VK_SPACE:
					m_pAvatar->ProcessAction(EAT);
					break;
				case VK_ESCAPE:
					m_pAvatar->ProcessAction(QUIT);
					break;
				default:
					break;
			};
		}
	}
}

void KeyBot::OnStop()
{
}
