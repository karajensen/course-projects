// * ************************************************************************ //
// * * MsgTypes header                                                    * * //
// * *   : defines message types for controller/controllable interface    * * //
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
// * * MsgTypes.h                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 13/03/11                      * * //
// * ********************************************************************** * //

#ifndef MSGTYPES_H
#define MSGTYPES_H

enum rel_directions	{FORWARD = 0, RIGHT, BACK, LEFT};
enum entity_types	{EMPTY = 0, FOG, WALL, HUNTER, SHROOM, MAGICSHROOM};
enum action_types	{QUIT = 0, STEP, TURNLEFT, TURNRIGHT, LOOK, EAT};

typedef rel_directions	REL_DIRECTION;
typedef entity_types	ENTITY;
typedef action_types	ACTION;

const unsigned int NUM_DIRECTIONS = 4;
const unsigned int NUM_ACTIONS = 6;

typedef struct _percept
{
	unsigned int health;
	ENTITY	view[NUM_DIRECTIONS];
	unsigned int fog_cleared;
} PERCEPT, *LPPERCEPT;


#endif

// ************************************************************************** //
