#include "bitboard.h"

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
