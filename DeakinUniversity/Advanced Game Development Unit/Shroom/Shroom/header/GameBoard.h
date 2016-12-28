// * ************************************************************************ //
// * * TGameBoard header                                                  * * //
// * *   : defines TGameBoard template class                              * * //
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
// * * TGameBoard.h                                                       * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef TGAMEBOARD_H
#define TGAMEBOARD_H

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

#include <memory>
#include <exception>

const unsigned int BUFSIZE = 256;

// define a new exception class for invalid matrix (2D) indices
class invalid_index : public std::exception
{
	private:
		unsigned int r, c;
		char szMsg[BUFSIZE];

	protected:
		invalid_index& operator=(const invalid_index &bi){}

	public:
		invalid_index(unsigned int const &_r, unsigned int const &_c)
			: r(_r), c(_c)  {}

		invalid_index(const invalid_index &bi)
		{
			r = bi.r;
			c = bi.c;
			strcpy_s(szMsg,BUFSIZE*sizeof(char),bi.szMsg);
		}

		const char * what()
		{
			// construct error msg only when needed
			sprintf_s(szMsg,BUFSIZE*sizeof(char),"Bad index value (%u,%u)",r,c);
			return szMsg;
		}
};



template <typename TileType>
class TGameBoard
{

	public:
		typedef TileType	TILE;

	private:
		unsigned int   m_uRows, m_uCols;
		TILE *m_pBoard;

		void DeleteBoard();
		unsigned int IndexOf(unsigned int r, unsigned int c);

	protected:
		TGameBoard(const TGameBoard&){}
		TGameBoard& operator=(const TGameBoard&){}

	public:
		TGameBoard();
		TGameBoard(unsigned int _rows, unsigned int _cols);
		~TGameBoard();

		void resize(unsigned int _rows, unsigned int _cols);

		TILE& operator()(unsigned int row, unsigned int col);
		TILE  operator()(unsigned int row, unsigned int col) const;

		TILE& operator()(COORD const & p);
		TILE  operator()(COORD const & p) const;

		const unsigned int& rows() const;
		const unsigned int& cols() const;

};

template <typename T>
TGameBoard<T>::TGameBoard()
: m_pBoard(NULL), m_uRows(0), m_uCols(0)
{
}


template <typename T>
TGameBoard<T>::TGameBoard(unsigned int _rows, unsigned int _cols)
: m_pBoard(NULL), m_uRows(_rows), m_uCols(_cols)
{
	if (m_uRows * m_uCols == 0)
		throw invalid_index(m_uRows,m_uCols);

	m_pBoard = new TILE[m_uRows*m_uCols];
	if (!m_pBoard)
		throw std::bad_alloc("Unable to allocate board memory");
}


template <typename T>
TGameBoard<T>::~TGameBoard()
{
	DeleteBoard();
}


template <typename T>
void TGameBoard<T>::DeleteBoard()
{
	if (m_pBoard)
	{
		delete [] m_pBoard;
		m_pBoard = NULL;
	}
}

template <typename T>
void TGameBoard<T>::resize(unsigned int _rows, unsigned int _cols)
{
	DeleteBoard();
	m_uRows = _rows;
	m_uCols = _cols;

	if (m_uRows * m_uCols == 0)
		throw invalid_index(m_uRows,m_uCols);

	m_pBoard = new TILE[m_uRows*m_uCols];
	if (!m_pBoard)
		throw std::bad_alloc("Unable to allocate board memory");
}


// row,col hash function (simple array index, column major form
template <typename T>
unsigned int TGameBoard<T>::IndexOf(unsigned int r, unsigned int c)
{
	if (r >= m_uRows || c >= m_uCols)
		throw invalid_index(r,c);
	return  c + r * m_uCols;
}

// Array dimension accessors
template <typename T>
const unsigned int& TGameBoard<T>::rows() const
{
	return m_uRows;
}

template <typename T>
const unsigned int& TGameBoard<T>::cols() const
{
	return m_uCols;
}


// Element access methods
template <typename T>
T& TGameBoard<T>::operator()(unsigned int row, unsigned int col)
{
	if (!m_pBoard)
		throw std::exception("Invalid board access: board memory not allocated");

	return m_pBoard[IndexOf(row,col)];
}

template <typename T>
T  TGameBoard<T>::operator()(unsigned int row, unsigned int col) const
{
	if (!m_pBoard)
		throw std::exception("Invalid board access: board memory not allocated");

	return m_pBoard[IndexOf(row,col)];
}

template <typename T>
T& TGameBoard<T>::operator()(COORD const & p)
{
	if (!m_pBoard)
		throw std::exception("Invalid board access: board memory not allocated");

	return m_pBoard[IndexOf(p.Y,p.X)];
}

template <typename T>
T  TGameBoard<T>::operator()(COORD const & p) const
{
	if (!m_pBoard)
		throw std::exception("Invalid board access: board memory not allocated");

	return m_pBoard[IndexOf(p.Y,p.X)];
}

#endif

// ************************************************************************** //
