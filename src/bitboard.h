#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <stdio.h>
#include <assert.h>
#include "helpers.h"

typedef unsigned int uint;
typedef unsigned long long int BitBoard;

void print_bitboard(BitBoard b);
void init_boards(uint boardsize);

/* precomputed global boards */
extern BitBoard boardmask;
extern BitBoard leftboards[64], rightboards[64], upboards[64], downboards[64];

/*
 *BitBoard leftboards[64], rightboards[64];
 *BitBoard upboards[64], downboards[64];
 */

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

#endif /* __BITBOARD_H__ */
