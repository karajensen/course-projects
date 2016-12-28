// * ************************************************************************ //
// * * GameTypes header                                                   * * //
// * *   : defines custom types for shroom game interface                 * * //
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
// * * GameTypes.h                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <iostream>

#include "MsgTypes.h"
#include "TArray.h"
#include "UtilityFunc.h"
#include "Config.h"

#if defined(DEBUG) || defined(_DEBUG)
	#include <assert.h>
#endif

enum map_directions	{NORTH = 0, EAST, SOUTH, WEST};
enum game_states	{PAUSED = 0, WIN, LOSE, RUNNING, REPLAY};

typedef map_directions	MAP_DIRECTION;
typedef game_states		GAMESTATE;
typedef int	OBJVALUE;

typedef struct _tile
{
	ENTITY		type;
	bool		visible;
	OBJVALUE	value;
} TILE, *LPTILE;

const int TOKEN[6] = {32, 176, 178, 1, 6, 5};
const char NOSE_TOKEN[NUM_DIRECTIONS] = {'^','>','v','<'};

typedef TArray2D<TILE>		GAMEBOARD;
typedef TArray2D<CHAR_INFO>	GUIBUFFER;

typedef struct _state
{
	COORD			position;
	MAP_DIRECTION	facing;
	OBJVALUE		health;
} HUNTERSTATE;

enum COLOUR {
	BLACK = 0x0000,
	FOREGROUND_CYAN		= FOREGROUND_GREEN	| FOREGROUND_BLUE,
	FOREGROUND_MAGENTA	= FOREGROUND_RED	| FOREGROUND_BLUE,
	FOREGROUND_YELLOW	= FOREGROUND_RED	| FOREGROUND_GREEN,
	FOREGROUND_GREY		= FOREGROUND_RED	| FOREGROUND_BLUE	| FOREGROUND_GREEN,
	FOREGROUND_WHITE	= FOREGROUND_GREY	| FOREGROUND_INTENSITY,
	BACKGROUND_CYAN		= BACKGROUND_GREEN	| BACKGROUND_BLUE,
	BACKGROUND_MAGENTA	= BACKGROUND_RED	| BACKGROUND_BLUE,
	BACKGROUND_YELLOW	= BACKGROUND_RED	| BACKGROUND_GREEN,
	BACKGROUND_GREY		= BACKGROUND_RED	| BACKGROUND_BLUE	| BACKGROUND_GREEN,
	BACKGROUND_WHITE	= BACKGROUND_GREY	| BACKGROUND_INTENSITY
};

#endif

// ************************************************************************** //
