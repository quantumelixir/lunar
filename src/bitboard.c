#include "bitboard.h"

BitBoard boardmask;
BitBoard leftboards[64], rightboards[64], upboards[64], downboards[64];

/*
 *BitBoard leftboards[64];
 *BitBoard rightboards[64];
 *BitBoard upboards[64];
 *BitBoard downboards[64];
 */

void print_bitboard(BitBoard b)
{
    int i, j;
    printf("---------------\n");
    for (i = 64; i > 0; i -= 8) {
        for (j = i - 8; j < i; ++j)
            printf("%s%s",  CHKBIT(b, j) ? "*" : ".", !((j + 1) % 8) ? "\n" : " ");
    }
    printf("---------------\n");
}

static void make_boardmask(uint boardsize)
{
	assert(boardsize > 0 && boardsize <= 8);
	boardmask = 0;
    int i, j;
    for (i = 0; i < boardsize; i++)
		for (j = 0; j < boardsize; j++)
			boardmask |= BIT(i*8 + j);
}

static void make_leftboards(uint boardsize)
{
    int i, j, k;
    for (i = 0; i < boardsize; i++)
		for (j = 0; j < boardsize; j++)
			for (k = j - 1; k >= 0; --k)
            	leftboards[i*8 + j] |= BIT(i*8 + k);
}

static void make_rightboards(uint boardsize)
{
    int i, j, k;
    for (i = 0; i < boardsize; i++)
		for (j = 0; j < boardsize; j++)
			for (k = j + 1; k < boardsize; ++k)
            	rightboards[i*8 + j] |= BIT(i*8 + k);
}

static void make_upboards(uint boardsize)
{
    int i, j, k;
    for (i = 0; i < boardsize; i++)
		for (j = 0; j < boardsize; j++)
			for (k = i + 1; k < boardsize; ++k)
            	upboards[i*8 + j] |= BIT(k*8 + j);
}

static void make_downboards(uint boardsize)
{
    int i, j, k;
    for (i = 0; i < boardsize; i++)
		for (j = 0; j < boardsize; j++)
			for (k = i - 1; k >= 0; --k)
            	downboards[i*8 + j] |= BIT(k*8 + j);
}

static void init_allzero(void)
{
	int i, j;
	boardmask = 0;
	for (i = 0; i < 8; ++i)
    	for (j = 0; j < 8; ++j) {
        	leftboards[8*i + j]  = 0;
        	rightboards[8*i + j] = 0;
        	upboards[8*i + j]    = 0;
        	downboards[8*i + j]  = 0;
        }
}

void init_boards(uint boardsize)
{
	init_allzero();
	make_boardmask(boardsize);
	make_leftboards(boardsize);
	make_rightboards(boardsize);
	make_upboards(boardsize);
	make_downboards(boardsize);
}
