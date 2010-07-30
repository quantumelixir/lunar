#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#include "bitboard.h"
#include "helpers.h"

int main()
{
    Game g;
    BitBoard humans   = 0xD004001000040057LLU;
    BitBoard bots     = 0x1500100010LLU;
    BitBoard occupied = humans | bots;

    print_bitboard(humans);
    print_bitboard(bots);
    print_bitboard(occupied);

    init_boards(5);
    print_bitboard(boardmask);

    return 0;
}
