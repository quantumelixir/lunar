#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#include "game.h"
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

    BitBoard test =  BITBOARD(
                        00001011,
                        00100000,
                        00000000,
                        00001000,
                        00000000,
                        00100000,
                        00000000,
                        11101010
                     );
    print_bitboard(test);

    // Example game state (letters are humans and numbers are bots)
    // . X . Y .
    // . . . . 1
    // . . T . .
    // Z . 2 . .
    // . . . 3 .

    Game game = { 5, 5,
                  BITBOARD(   // blocks
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00000000
                  ),
                  BITBOARD(   // target
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00000000,
                    00100000,
                    00000000,
                    00000000
                  )
                }, ngame;
    State state = {
                    BITBOARD(   // humans
                        00000000,
                        00000000,
                        00000000,
                        01010000,
                        00000000,
                        00000000,
                        10000000,
                        00000000
                    ),
                    BITBOARD(   // robots
                        00000000,
                        00000000,
                        00000000,
                        00000000,
                        00001000,
                        00000000,
                        00100000,
                        00010000
                    )
                  }, nstate;

    printf("Writing to file %s\n",
            save_game("example", &game, &state) ? "DONE!" : "FAILED!");

    printf("Reading from file %s\n",
            load_game("example", &ngame, &nstate) ? "DONE!" : "FAILED!");

    printf("Read-Write Test: %s\n",
            ngame.r == game.r &&
            ngame.c == game.c &&
            ngame.blocks == game.blocks &&
            ngame.target == game.target &&
            nstate.humans == state.humans &&
            nstate.robots == state.robots
            ? "PASSED!" : "FAILED!");

    print_game(&ngame, &nstate);

    return 0;
}
