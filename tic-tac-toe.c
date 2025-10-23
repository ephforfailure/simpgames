#include <raylib.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>


/*
 *	Game Logic
 */

/* NOTE The code will disable cross wins if these width and height aren't the same. */
#define BOARD_WIDTH  3
#define BOARD_HEIGHT 3

#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 300

typedef enum PLAYER_E {
	PLAYER_ZERO,
	PLAYER_X,
	PLAYER_O,
} player_e;

typedef enum GAME_STATE_E {
	GAME_INACTIVE,
	GAME_ACTIVE,
	GAME_TIE,

	GAME_WIN = 1000,
	GAME_WIN_ROW,
	GAME_WIN_COLUMN,
	GAME_WIN_CROSS_LEFT,
	GAME_WIN_CROSS_RIGHT,
} game_state_e;

typedef struct {
	player_e  current_player;
	player_e* board;
	game_state_e state;
	int win_offset;

} game_t;

int board_area(void) {
	return BOARD_WIDTH * BOARD_HEIGHT;
}

game_t* new_game(void) {
	game_t* game = malloc(sizeof(game_t));
	assert(game);
	game->board = malloc(sizeof(player_e) * board_area());
	assert(game->board);
	int i = 0;
	for (;i<board_area(); i++) {
		game->board[i] = PLAYER_ZERO;
	}
	game->current_player = PLAYER_X;
	game->state = GAME_INACTIVE;
	game->win_offset = -127;
	return game;
}

int board_get_index(int x, int y) {
	int index = BOARD_WIDTH * y;
	index += x;
	assert(board_area() > index);
	return index;
}

bool game_is_cell_zero(game_t* game, int x, int y) {
	int index = board_get_index(x, y);
	return game->board[index] == PLAYER_ZERO;
}

void game_move(game_t* game, int x, int y) {
	if (game_is_cell_zero(game, x, y)) {
		int index = board_get_index(x, y);
		game->board[index] = game->current_player;
	} else {
		assert(false);
	}
}

void game_switch_player(game_t* game) {
	if (game->current_player == PLAYER_X) {
		game->current_player = PLAYER_O;
	} else {
		game->current_player = PLAYER_X;
	}
}

bool game_is_over(game_t* game) {

	bool is_won;
	int y, x, index = 0;

	bool tie = true;
	for (index=0; index<board_area(); index++){
		player_e player = game->board[index];
		if (player == PLAYER_ZERO) {
			tie = false;
			break;
		}
	}
	if (tie) {
		game->state = GAME_TIE;
		return tie;
	}

	/* check for row wins */
	for (y=0; y<BOARD_WIDTH; y++) { 
		is_won = true;
		index = board_get_index(0, y);
		player_e player = game->board[index];
		for (x=0; x<BOARD_HEIGHT; x++) {
			index = board_get_index(x, y);
			if (player != game->board[index]) {
				is_won = false;
				break;
			}
		}
		if (is_won) {
			game->state = GAME_WIN_ROW;
			game->win_offset = y;
			return is_won;
		}
	}

	/* check for column wins */
	for (x=0; x<BOARD_HEIGHT; x++) { 
		is_won = true;
		index = board_get_index(x, 0);
		player_e player = game->board[index];
		for (y=0; y<BOARD_WIDTH; y++) {
			index = board_get_index(x, y);
			if (player != game->board[index]) {
				is_won = false;
				break;
			}
		}
		if (is_won) {
			game->state = GAME_WIN_COLUMN;
			game->win_offset = x;
			return is_won;
		}
	}

	if (BOARD_HEIGHT != BOARD_WIDTH) { // ignore cross wins if the board is not squared.
		return false;
	}

	/* check for cross wins */

	/* -------------
	 * | 0 | 1 | 2 |
	 * | 3 | 4 | 5 |
	 * | 6 | 7 | 8 |
	 * -------------
	 *  left:  0, 4, 8
	 *  right: 2, 4, 6 
	 */

	/* check for left cross win */
	int offset;

	player_e player = game->board[0];
	offset = BOARD_WIDTH + 1;
	is_won = true;
	for (index=0; index<board_area(); index += offset) {
		if (player != game->board[index]){
			is_won = false;
			break;
		}
	}
	if (is_won) {
		game->state = GAME_WIN_CROSS_LEFT;
		return is_won;
	}


	/* check for right cross win */
	offset = BOARD_WIDTH - 1;
	is_won = true;
	for (index=offset; index<board_area(); index += offset) {
		if (player != game->board[index]) {
			is_won = false;
			break;
		}
	}
	if (is_won) {
		game->state = GAME_WIN_CROSS_RIGHT;
		return is_won;
	}
	return false;
}

int cell_width_offset(void) {
	return WINDOW_WIDTH / BOARD_WIDTH;
} 
int cell_height_offset(void) {
	return WINDOW_HEIGHT / BOARD_HEIGHT;
}

void DrawGameGrid(game_t *game) {
	int x = 0;
	for (x=0; x<BOARD_WIDTH; x++) {
		int x_point = x * cell_width_offset();
		DrawLine(x_point, 0, x_point, WINDOW_HEIGHT, BLACK);
	}
	int y=0;
	for (y=0; y<BOARD_HEIGHT; y++) {
		int y_point = y * cell_height_offset();
		DrawLine(0, y_point, WINDOW_WIDTH, y_point, BLACK);
	}
}

int main(void) {

	game_t* game = new_game();
	
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tic-Tac-Toe");

	Color bg_color = RED;

	while (!WindowShouldClose()) {
		ClearBackground(bg_color);
		BeginDrawing();
			DrawGameGrid(game);
		EndDrawing();
	}
}
