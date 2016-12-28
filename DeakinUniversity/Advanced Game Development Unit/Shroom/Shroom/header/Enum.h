#ifndef ENUM_H
#define ENUM_H

enum map_direction {NORTH = 0, EAST, SOUTH, WEST};
enum rel_direction {FORWARD, BACK, LEFT, RIGHT};
enum game_state {PAUSED = 0, WIN, LOSE, RUNNING, REPLAY};
enum entity_type {EMPTY = 0, FOG, WALL, HUNTER, SHROOM, MAGICSHROOM};
enum action_type {NONE = 0, STEP, TURNLEFT, TURNRIGHT, LOOK, EAT};
#endif