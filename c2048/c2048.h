#pragma once
#ifndef C2048_H
#define C2048_H

#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 4
#define GAME_BASE  2
#define PAD_WIDTH  4

#define BOARD_AREA (BOARD_SIZE * BOARD_SIZE)

static_assert(BOARD_SIZE >= 2, "Board size must be at least 2x2");
static_assert(PAD_WIDTH >= 4, "Pad width is too small");
static_assert(BOARD_AREA <= RAND_MAX, "Board area is larger than RAND_MAX");

typedef unsigned short NUMBER;
typedef unsigned long SCORE;
typedef char TILE_INDEX;
typedef TILE_INDEX TILE_COUNT;

typedef unsigned char BOOLEAN; // c does not have bools :(
#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined
#define true 1
#define false 0
#endif // __bool_true_false_are_defined

static NUMBER board[BOARD_AREA];
static SCORE score;

typedef enum {
	INVALID_DIRECTION,
	UP,
	DOWN,
	LEFT,
	RIGHT
} MOVE_DIRECTION;

void setup_game(void);
void display_board(void);
TILE_COUNT count_present_tiles(void);
TILE_COUNT count_empty_tiles(void);
BOOLEAN add_random_tile(void);
NUMBER random_spawn_value(void);
void move_board(MOVE_DIRECTION direction, NUMBER* dst_board, SCORE* dst_score);
BOOLEAN is_move_valid(MOVE_DIRECTION direction);
BOOLEAN will_any_move(MOVE_DIRECTION direction);
BOOLEAN will_any_merge(MOVE_DIRECTION direction);
BOOLEAN any_valid_moves(void);

#endif // C2048_H