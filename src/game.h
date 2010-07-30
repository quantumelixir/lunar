#ifndef __GAME_H__
#define __GAME_H__

#include "bitboard.h"
#include <stdio.h>

/* initalize bitboards to zero */
void init_boards(uint boardsize);

/* precomputed global boards */
extern BitBoard boardmask;
extern BitBoard leftboards[64], rightboards[64], upboards[64], downboards[64];

/* General Game information */
typedef struct
{
    uint r, c;
    BitBoard blocks;
    BitBoard target;
}Game;

/* State specific information */
typedef struct
{
    BitBoard humans;
    BitBoard robots;
}State;

void print_game(const Game* game, const State* state);
int load_game(const char* filename, Game* game, State* state);
int save_game(const char* filename, const Game* game, const State* state);


#endif /* __GAME_H__ */
