// * ************************************************************************ //
// * * UtilityFunc Source                                                 * * //
// * *   : defines utility function implementations                       * * //
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
// * * UtilityFunc.cpp                                                    * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#include "UtilityFunc.h"
#include <stdlib.h>
#include <cmath>

COORD operator+(COORD const& lhs, COORD const& rhs)
{
	return lhs+=rhs;
}

COORD operator-(COORD const& lhs, COORD const& rhs)
{
	return lhs-=rhs;
}

COORD operator+=(COORD const& lhs, COORD const& rhs)
{
	COORD c = lhs;
	c.X += rhs.X;
	c.Y += rhs.Y;
	return c;
}

COORD operator-=(COORD const& lhs, COORD const& rhs)
{
	COORD c = lhs;
	c.X -= rhs.X;
	c.Y -= rhs.Y;
	return c;
}

bool   operator==(COORD const& lhs, COORD const& rhs)
{
	return (lhs.X==rhs.X && lhs.Y==rhs.Y);
}

bool   operator!=(COORD const& lhs, COORD const& rhs)
{
	return !operator==(lhs,rhs);
}

COORD operator-(COORD const& c)
{
	COORD nc;
	nc.X = -c.X;
	nc.Y = -c.Y;
	return nc;
}

COORD	RandomCoordinate(COORD const& lower, COORD const& upper)
{
	SHORT dx = upper.X - lower.X;
	SHORT dy = upper.Y - lower.Y;

	COORD p;

	p.X = lower.X + rand() % dx;
	p.Y = lower.Y + rand() % dy;

	return p;
}

float distance(COORD const &p1, COORD const &p2)
{
	SHORT dx = p2.X - p1.X;
	SHORT dy = p2.Y - p1.Y;

	return sqrt( float(dx*dx+dy*dy) );
}

void ClearConsole(HANDLE hStdOut)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
	GetConsoleScreenBufferInfo(hStdOut,&csbiConsoleInfo);
	SMALL_RECT srcWindow = csbiConsoleInfo.srWindow;
	DWORD dNumChars = (srcWindow.Right - srcWindow.Left+1)*(srcWindow.Bottom-srcWindow.Top+1);
	COORD cStart;
	cStart.X = srcWindow.Left;
	cStart.Y = srcWindow.Top;
	FillConsoleOutputCharacter(hStdOut,' ',dNumChars,cStart,&dNumChars);
}