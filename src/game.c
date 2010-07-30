#include "game.h"

BitBoard boardmask;
BitBoard leftboards[64], rightboards[64], upboards[64], downboards[64];

/* prints a nice game board with
 *
 * a, b, ... letter pieces for humans
 * 1, 2, ... numbered pieces for robots
 * # hashes for blocks
 * @ squiggly for targets
 *
 * When a letter piece is on a target the
 * corresponding capital letter is printed
 *
 * When a numbered piece is on a target we
 * simply print the numbered piece overshadowing
 * the target at that location
 */
void print_game(const Game* game, const State* state)
{
    int i, j, humancount = 0, robotcount = 0;
    int ontarget;
    char c;

    printf(".");
    for (i = 0; i <= 2*game->c; ++i) printf("-");
    printf(".\n");

    for (i = 8 * game->r; i > 0; i -= 8) {
        printf("| ");
        for (j = i - 8; j < i - 8 + game->c; ++j) {
            ontarget = CHKBIT(game->target, j);
            c = CHKBIT(game->blocks, j) ? '#' :
                CHKBIT(state->humans, j) && ontarget ? 'a' + humancount++ :
                CHKBIT(state->robots, j) && ontarget ? '1' + robotcount++ :
                ontarget ? '@' :
                CHKBIT(state->humans, j) ? 'A' + humancount++ :
                CHKBIT(state->robots, j) ? '1' + robotcount++ :
                '.';
            printf("%c%s", c, !((j - i + 9) % game->c) ? " |\n" : " ");
        }
    }

    printf(".");
    for (i = 0; i <= 2*game->c; ++i) printf("-");
    printf(".\n");

    printf("Board Dimensions: %dx%d\n", game->r, game->c);
    printf("Blocks          : %d\n", __builtin_popcount(game->blocks));
    printf("Targets         : %d\n", __builtin_popcount(game->target));
    printf("Human Count     : %d\n", __builtin_popcount(state->humans));
    printf("Robot Count     : %d\n", __builtin_popcount(state->robots));
    printf("\n");
}

int load_game(const char* filename, Game* game, State* state)
{
    FILE* fin = fopen(filename, "rb");
    if (!fin) // error opening file
        return 0;

    fread(game, sizeof(Game), 1, fin);
    fread(state, sizeof(State), 1, fin);

    fclose(fin);
    return 1;
}

int save_game(const char* filename, const Game* game, const State* state)
{
    FILE* fout = fopen(filename, "wb");
    if (!fout) // error opening file
        return 0;

    fwrite(game, sizeof(Game), 1, fout);
    fwrite(state, sizeof(State), 1, fout);

    fclose(fout);
    return 1;
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
