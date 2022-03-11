// c2048.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include "getch_raw.h"

#include "c2048.h"

int main(int argc, char* argv[])
{
	init_getch_raw();
	setup_game();

	// game loop
	for(;;){
		char input;
		MOVE_DIRECTION direction;

		display_board();
		printf(">");
		input = getch_raw();
		if(input == 0 || input == -32) // if this is escape then getch again for arrow key
			input = getch_raw();
		printf("\n");
		direction = INVALID_DIRECTION;
		switch(input){
			case 'W':
			case 'w':
			case KEY_ARROW_UP:
				direction = UP;
				break;
			case 'A':
			case 'a':
			case KEY_ARROW_LEFT:
				direction = LEFT;
				break;
			case 'S':
			case 's':
			case KEY_ARROW_DOWN:
				direction = DOWN;
				break;
			case 'D':
			case 'd':
			case KEY_ARROW_RIGHT:
				direction = RIGHT;
				break;
		}

		// check if move is possible
		if(!is_move_valid(direction)) direction = INVALID_DIRECTION;

		// move
		if(direction==INVALID_DIRECTION){
			printf("\a\nInvalid direction. Please try again.\n");
			continue;
		}
		move_board(direction, NULL, &score);

		(void) add_random_tile(); // add a tile now we have made a move
		if(count_present_tiles() == BOARD_AREA && !any_valid_moves()) break; // if board is full and nothing can be merged, exit
	}

	// end
	display_board();
	printf("\a\nGame over.\nYou scored %lu! Press any key to exit.", score);
	(void) getchar(); // wait for key
	return 0;
}

void setup_game(void){
	// seed random number generator with the current time
	srand((unsigned int)time(NULL));

	// clear the board
	memset(board, 0, sizeof(board));
	// reset the score
	score = 0;

	// add two random tiles
	(void) add_random_tile();
	(void) add_random_tile();
}

void display_board(void){
	TILE_INDEX row;
	TILE_INDEX col;

	printf("Score: %lu\n", score);
	for(row=0; row < BOARD_SIZE; row++){
		for(col=0; col < BOARD_SIZE; col++){
			if(board[col + (row * BOARD_SIZE)])
				printf("%-*d", PAD_WIDTH, board[col + (row * BOARD_SIZE)]);
			else
				printf("%-*c", PAD_WIDTH, ' ');
			if(col < BOARD_SIZE - 1)
				printf("|");
			else
				printf("\n");
		}
	}
}

TILE_COUNT count_present_tiles(void){
    TILE_COUNT count = 0;
	TILE_INDEX i;
	for(i=0; i<BOARD_AREA; i++){
		if(board[i])
			count++;
	}
    return count;
}

TILE_COUNT count_empty_tiles(void){
    TILE_COUNT count = 0;
	TILE_INDEX i;
	for(i=0; i<BOARD_AREA; i++){
		if(!board[i])
			count++;
	}
    return count;
}

BOOLEAN add_random_tile(void){
    TILE_COUNT empty = count_empty_tiles();
    TILE_INDEX insert_position;
	TILE_COUNT passed = 0;
	TILE_INDEX i;

	if(empty == 0) return false;
	insert_position = rand() % empty;

	for(i=0;;i++){
		if(!board[i]){
			if(passed == insert_position){
				board[i] = random_spawn_value();
				return true;
			}
			passed++;
		}
	}
}

NUMBER random_spawn_value(void){
	return GAME_BASE * (2-((rand() % 10) < 9));
}

void move_board(MOVE_DIRECTION direction, NUMBER* dst_board, SCORE* dst_score){
	//store which cells have already been merged so we don't do it twice
	unsigned char merged[BOARD_AREA]; // a little faster than bit packing because shifting would take time

	if(direction == INVALID_DIRECTION) return;
	// if dst_board is specified, copy the board into there
	if(dst_board == NULL) dst_board = board;
	else memcpy(dst_board, board, sizeof(board));

	if(dst_score) *dst_score = score;

	// clear the merged list
	memset(merged, 0, sizeof(merged));

	switch(direction){
		TILE_INDEX col;
		TILE_INDEX row;
		case UP:
			for(col=0; col<BOARD_SIZE; col++){
				for(row=1; row<BOARD_SIZE;row++){
					if(dst_board[col + row*BOARD_SIZE]){
						while(row>0 && !dst_board[col + (row-1)*BOARD_SIZE]){ // while above cell is empty
							// shift up
							dst_board[col + (row-1)*BOARD_SIZE] = dst_board[col + row*BOARD_SIZE];
							dst_board[col + row*BOARD_SIZE] = 0; //clear
							row--;
						}
						if(row>0 && dst_board[col + (row-1)*BOARD_SIZE] == dst_board[col + row*BOARD_SIZE]
									&& !merged[col + (row-1)*BOARD_SIZE]){
							// merge up
							dst_board[col + (row-1)*BOARD_SIZE] *= 2; // double
							dst_board[col + row*BOARD_SIZE] = 0; // clear
							// mark as merged
							merged[col + (row-1)*BOARD_SIZE] = true;
							// add to score
							if(dst_score) *dst_score += dst_board[col + (row-1)*BOARD_SIZE];
						}
					}
				}
			}
			break;
		case LEFT:
			for(row=0; row<BOARD_SIZE; row++){
				for(col=1; col<BOARD_SIZE;col++){
					if(dst_board[col + row*BOARD_SIZE]){
						while(col>0 && !dst_board[(col-1) + row*BOARD_SIZE]){ // while left cell is empty
							// shift left
							dst_board[(col-1) + row*BOARD_SIZE] = dst_board[col + row*BOARD_SIZE];
							dst_board[col + row*BOARD_SIZE] = 0; //clear
							col--;
						}
						if(col>0 && dst_board[(col-1) + row*BOARD_SIZE] == dst_board[col + row*BOARD_SIZE]
									&& !merged[(col-1) + row*BOARD_SIZE]){
							// merge left
							dst_board[(col-1) + row*BOARD_SIZE] *= 2; // double
							dst_board[col + row*BOARD_SIZE] = 0; // clear
							// mark as merged
							merged[(col-1) + row*BOARD_SIZE] = true;
							// add to score
							if(dst_score) *dst_score += dst_board[(col-1) + row*BOARD_SIZE];
						}
					}
				}
			}
			break;
		case DOWN:
			for(col=0; col<BOARD_SIZE; col++){
				for(row=BOARD_SIZE-2; row>=0;row--){
					if(dst_board[col + row*BOARD_SIZE]){
						while(row<BOARD_SIZE-1 && !dst_board[col + (row+1)*BOARD_SIZE]){ // while below cell is empty
							// shift down
							dst_board[col + (row+1)*BOARD_SIZE] = dst_board[col + row*BOARD_SIZE];
							dst_board[col + row*BOARD_SIZE] = 0; //clear
							row++;
						}
						if(row<BOARD_SIZE-1 && dst_board[col + (row+1)*BOARD_SIZE] == dst_board[col + row*BOARD_SIZE]
									&& !merged[col + (row+1)*BOARD_SIZE]){
							// merge down
							dst_board[col + (row+1)*BOARD_SIZE] *= 2; // double
							dst_board[col + row*BOARD_SIZE] = 0; // clear
							// mark as merged
							merged[col + (row+1)*BOARD_SIZE] = true;
							// add to score
							if(dst_score) *dst_score += dst_board[col + (row+1)*BOARD_SIZE];
						}
					}
				}
			}
			break;
		case RIGHT:
			for(row=0; row<BOARD_SIZE; row++){
				for(col=BOARD_SIZE-2; col>=0;col--){
					if(dst_board[col + row*BOARD_SIZE]){
						while(col<BOARD_SIZE-1 && !dst_board[(col+1) + row*BOARD_SIZE]){ // while right cell is empty
							// shift right
							dst_board[(col+1) + row*BOARD_SIZE] = dst_board[col + row*BOARD_SIZE];
							dst_board[col + row*BOARD_SIZE] = 0; //clear
							col++;
						}
						if(col<BOARD_SIZE-1 && dst_board[(col+1) + row*BOARD_SIZE] == dst_board[col + row*BOARD_SIZE]
									&& !merged[(col+1) + row*BOARD_SIZE]){
							// merge left
							dst_board[(col+1) + row*BOARD_SIZE] *= 2; // double
							dst_board[col + row*BOARD_SIZE] = 0; // clear
							// mark as merged
							merged[(col+1) + row*BOARD_SIZE] = true;
							// add to score
							if(dst_score) *dst_score += dst_board[(col+1) + row*BOARD_SIZE];
						}
					}
				}
			}
			break;
	};
}

BOOLEAN is_move_valid(MOVE_DIRECTION direction){
	BOOLEAN x = will_any_move(direction);
	BOOLEAN y = will_any_merge(direction);
	return (direction != INVALID_DIRECTION) && (x || y);
}

BOOLEAN will_any_merge(MOVE_DIRECTION direction){
	if(direction == INVALID_DIRECTION) return false;
	switch(direction){
		TILE_INDEX col;
		TILE_INDEX row;
		case UP:
			for(col=0; col<BOARD_SIZE; col++){
				for(row=1; row<BOARD_SIZE;row++){
					if(board[col + row*BOARD_SIZE] && 
							board[col + row*BOARD_SIZE] == board[col + (row-1)*BOARD_SIZE])
						return true;
				}
			}
			break;
		case LEFT:
			for(row=0; row<BOARD_SIZE; row++){
				for(col=1; col<BOARD_SIZE;col++){
					if(board[col + row*BOARD_SIZE] && 
							board[col + row*BOARD_SIZE] == board[(col-1) + row*BOARD_SIZE])
						return true;
				}
			}
			break;
		case DOWN:
			for(col=0; col<BOARD_SIZE; col++){
				for(row=BOARD_SIZE-2; row>=0;row--){
					if(board[col + row*BOARD_SIZE] && 
							board[col + row*BOARD_SIZE] == board[col + (row+1)*BOARD_SIZE])
						return true;
				}
			}
			break;
		case RIGHT:
			for(row=0; row<BOARD_SIZE; row++){
				for(col=BOARD_SIZE-2; col>=0;col--){
					if(board[col + row*BOARD_SIZE] && 
							board[col + row*BOARD_SIZE] == board[(col+1) + row*BOARD_SIZE])
						return true;
				}
			}
			break;
	};
	return false;
}

BOOLEAN will_any_move(MOVE_DIRECTION direction){
	if(direction == INVALID_DIRECTION) return false;
	switch(direction){
		TILE_INDEX col;
		TILE_INDEX row;
		case UP:
			for(col=0; col<BOARD_SIZE; col++){
				BOOLEAN found_empty = false;
				for(row=0; row<BOARD_SIZE;row++){
					if(!board[col + row*BOARD_SIZE])
						found_empty = true;
					else if(found_empty)
						return true;
				}
			}
			break;
		case LEFT:
			for(row=0; row<BOARD_SIZE; row++){
				BOOLEAN found_empty = false;
				for(col=0; col<BOARD_SIZE;col++){
					if(!board[col + row*BOARD_SIZE])
						found_empty = true;
					else if(found_empty)
						return true;
				}
			}
			break;
		case DOWN:
			for(col=0; col<BOARD_SIZE; col++){
				BOOLEAN found_empty = false;
				for(row=BOARD_SIZE-1; row>=0;row--){
					if(!board[col + row*BOARD_SIZE])
						found_empty = true;
					else if(found_empty)
						return true;
				}
			}
			break;
		case RIGHT:
			for(row=0; row<BOARD_SIZE; row++){
				BOOLEAN found_empty = false;
				for(col=BOARD_SIZE-1; col>=0;col--){
					if(!board[col + row*BOARD_SIZE])
						found_empty = true;
					else if(found_empty)
						return true;
				}
			}
			break;
	};
	return false;
}

BOOLEAN any_valid_moves(void){
	return is_move_valid(UP) || is_move_valid(LEFT) || is_move_valid(DOWN) || is_move_valid(RIGHT);
}