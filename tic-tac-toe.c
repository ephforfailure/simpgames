#include <raylib.h>
#include <stdlib.h>
#include <assert.h>
#include <bool.h>
#include <math.h>


#define BOARD_WIDTH  3
#define BOARD_HEIGHT 3

typedef enum {
	PLAYER_ZERO;
	PLAYER_X;
	PLAYER_O;
} player_e;

typedef struct {
	player_e current_player;
	player_e* board;
} game_t;

game_t* new_game(void) {
	game_t* game = malloc(sizeof(game_t));
	assert(game);
	game.table = malloc(sizeof(player_e) * BOARD_WIDTH * BOARD_WIDTH);
	assert(game.table);
	int i = 0;
	for (;i<(BOARD_HEIGHT*BOARD_WIDTH); i++) {
		game.table[i] = PLAYER_ZERO;
	}
	game.current_player = PLAYER_X;
	return game;
}

int board_get_index(int x, int y) {
	int index = BOARD_WIDTH * y;
	index += x;
	assert((BOARD_WIDTH*BOARD_HEIGHT) > index);
	return index;
}

bool game_is_cell_zero(game_t* game, int x, int y) {
	int index = board_get_index(x, y);
	return game.board[index] == PLAYER_ZERO;
}

void game_move(game_t* game, int x, int y) {
	if game_is_cell_zero(game, x, y) {
		int index = board_get_index(x, y);
		game.board[index] = game.current_player;
	} else {
		assert(false);
	}
}

void game_switch_player(game_t* game) {
	if game.current_player == PLAYER_X {
		game.current_player = PLAYER_O;
	} else {
		game.current_player = PLAYER_X;
	}
}

bool game_is_won(game_t* game) {
	
}




int main(void) {

	game_t* game = new_game();

	
	printf("hello, world!\n");

	InitWindow(300, 300, "HelloWorld");


}
