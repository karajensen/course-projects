// * ************************************************************************ //
// * * ShroomGUI source                                                   * * //
// * *   : implimentation of console GUI elements                         * * //
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
// * * ShroomGUI.cpp                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 26/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //

#include "ShroomGUI.h"


GUIPanel::GUIPanel(HANDLE hOut, SHORT top, SHORT left, SHORT bottom, SHORT right)
: m_ciBuffer(), m_hOut(hOut), m_fpCallback(0)
{
	m_srRegion.Top = top;
	m_srRegion.Bottom = bottom;
	m_srRegion.Left = left;
	m_srRegion.Right = right;

	SHORT rows = bottom - top + 1;
	SHORT cols = right - left + 1;
	m_ciBuffer.resize(rows,cols);
}

GUIPanel::GUIPanel(HANDLE hOut, COORD2 pos, SHORT rows, SHORT cols)
: m_ciBuffer(), m_hOut(hOut), m_fpCallback(0)
{
	m_srRegion.Top = pos.row;
	m_srRegion.Left = pos.col;
	m_srRegion.Bottom = pos.row + rows - 1;
	m_srRegion.Right = pos.col + cols - 1;

	m_ciBuffer.resize(rows,cols);
}

GUIPanel::~GUIPanel()
{
	SAFE_DELETE(m_fpCallback);
}

void GUIPanel::Draw()
{
	if (m_hOut == INVALID_HANDLE_VALUE)
		return;
	COORD pos = { 0, 0 };
	SMALL_RECT srDrawRegion = m_srRegion;
	WriteConsoleOutput(m_hOut, m_ciBuffer.buffer(), (COORD)m_ciBuffer.size(), pos, &srDrawRegion);
}

void GUIPanel::SetCallback(GUIPanel::FPCALLBACK fpFunc)
{
	SAFE_DELETE(m_fpCallback);
	m_fpCallback = fpFunc;
}

void GUIPanel::notify()
{
	(*m_fpCallback)(m_ciBuffer);
	Draw();
}

