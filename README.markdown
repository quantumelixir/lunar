# Lunar Lockout

## Setting

A *r*x*c* game board with
*m* humans
*n* robots
*b* blocks
1 target (Any position. Usually center)

## Rules
Objective is to get all humans to the target/exit in least number of
moves/steps. Blocks don't move. Robots and humans do; till they find an
obstacle(robot/block/human) in that direction and stop exactly one square
before it in that direction. Without an obstacle in the direction, a piece
(human/robot) cannot move in that direction.

## Implementation

### Board Representation
Internally the game state is represented using bitboards. This imposes
conditions on the size of the board (maximum board size is 8x8) so that the
move generation can be efficient. This doesn't mean a significant loss as most
interesting puzzles (human solvable ones) exist in dimensions smaller than 8x8.
Smaller boards are handled through appropriate bit masks.

<pre>
     Board                Bit Mapping
0 0 0 0 1 0 1 1  |  56 57 58 59 60 61 62 63 
0 0 1 0 0 0 0 0  |  48 49 50 51 52 53 54 55 
0 0 0 0 0 0 0 0  |  40 41 42 43 44 45 46 47 
0 0 0 0 1 0 0 0  |  32 33 34 35 36 37 38 39 
0 0 0 0 0 0 0 0  |  24 25 26 27 28 29 30 31 
0 0 1 0 0 0 0 0  |  16 17 18 19 20 21 22 23 
0 0 0 0 0 0 0 0  |   8  9 10 11 12 13 14 15 
1 1 1 0 1 0 1 0  |   0  1  2  3  4  5  6  7

Binary = 1101 0000 0000 0100 0000 0000 0001 0000 0000 0000 0000 0100 0000 0000 0101 0111
Hex    = 0xD004001000040057
</pre>

The game state is encoded in the following structure:
<pre>
struct GameState {
	Bitboard target; // has exactly one bit set
	Bitboard humans; // has m bits set
	Bitboard robots; // has n bits set
	Bitboard blocks; // has b bits set
};
</pre>

### Move Generation
The important move generation step is enumerating the possible moves for a
game piece (human/robot) given the game state.

Given a piece in the bitboard we can generate all its possible moves as follows:

That is, given this game state
<pre>
0 0 0 1 0
0 0 0 0 0
1 0 1 0 1
0 0 0 0 0
1 1 0 1 0
</pre>

we want to find a move for this piece to the right:
<pre>
0 0 0 0 0
0 0 0 0 0
1 0 0 0 0
0 0 0 0 0
0 0 0 0 0
</pre>

Let's say we have a handy lookup table where we store a pattern for every cell
in the game board. The pattern is a series of ones to the right of the cell. So
by indexing the table with the address of the cell we get this:
<pre>
0 0 0 0 0
0 0 0 0 0
0 1 1 1 1
0 0 0 0 0
0 0 0 0 0
</pre>

If we AND this with the occupied (blocks | robots | humans) bitboard we get this:
<pre>
0 0 0 0 0
0 0 0 0 0
0 0 1 0 1
0 0 0 0 0
0 0 0 0 0
</pre>

We know that the piece can only move to the right, so long as it doesn't run
into an obstacle. Here there are two obstacles to our right and we need to find
the leftmost one. Our destination will be one position to the left of the
leftmost obstacle.

To find the leftmost obstacle we keep left-shifting the previous bitboard and
OR-ing the result, as many times as the number of columns in the game board
minus two. In this example this is 5 - 2 = 3. Essentially we are smearing the
obstacles to the right long enough so that even if an obstacle is right next to
the piece in consideration it smears all the way till the edge of the board.
That's how we get the number of columns minus two rule.

After three such operations we have this:
<pre>       
0 0 0 0 0   0 0 0 0 0   0 0 0 0 0
1 0 0 0 0   1 1 0 0 0   1 1 1 0 0
0 0 1 1 1   0 0 1 1 1   0 0 1 1 1
0 0 0 0 0   0 0 0 0 0   0 0 0 0 0
0 0 0 0 0   0 0 0 0 0   0 0 0 0 0
</pre>        

Now since we are interested in only moving right, which is moving along a row,
we AND the bitboard with an all one bitboard for this row yielding this:
<pre>
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
1 1 1 0 0       0 0 0 0 0       0 0 0 0 0
0 0 1 1 1  AND  1 1 1 1 1   =   0 0 1 1 1
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
</pre>

Using the above bitboard we can XOR it with the bitboard for all ones to right
of the piece:
<pre>
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
0 0 1 1 1  XOR  0 1 1 1 1   =   0 1 0 0 0
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
0 0 0 0 0       0 0 0 0 0       0 0 0 0 0
</pre>

yielding the final position our piece can move to towards right:
<pre>
0 0 0 0 0
0 0 0 0 0
0 1 0 0 0
0 0 0 0 0
0 0 0 0 0
</pre>

In summary,

<pre>
right_obstacles  = all_ones_to_right & obstacles
smear_right     |= (right_obstacles << 1)
smear_right     |= (right_obstacles << 2)
smear_right     |= (right_obstacles << 3)
final_position   = smear_right ^ right_obstacles
</pre>

Using the above method, final_position will be an all 0 bitboard only when either

 * there are no obstacles to the right of the piece or
 * there is an obstacle right next to the piece

both of which represent invalid moves in our game which can be skipped.

Similar bitlevel operations are used to find the moves in the other directions
(up, down, left). 

### Technicalities
Other details like using board masks for non-8x8 boards also have to be taken
into account while performing the operations. The speciality of 8x8 boards is
that the native types on today's microprocessors are 64-bit integers, which
neatly map to the 8x8 boards. Other boards have to be forced into a bitboard
representation with a bit mask. So in the example above, manipulations with the
5x5 bitboard are purely hypothetical as bitshifting will have to be
specifically taken care of (there are no native 25-bit data types to play
with).

For such cases we use a mask to represent the valid regions of the bitboard.
<pre>
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
1 1 1 1 1 0 0 0
1 1 1 1 1 0 0 0
1 1 1 1 1 0 0 0
1 1 1 1 1 0 0 0
1 1 1 1 1 0 0 0
</pre>

The AND, OR, NOT, XOR operations remain unmodified in such cases. Moving a
piece up (left-shifting by 8) and moving a piece down (right-shifting by 8)
also remain unmodified. We only have to take care of left-shifting and
right-shifting on the smaller board.

Left-shifting:
Instead of the usual,
<pre>bitarray <<= 1</pre>
we check for boundary conditions:
<pre>
if ((~board_mask) & (bitarray << 1)) // outside board
	bitarray = ((bitarray << 1) | one_in_next_row_first_col)
else                                      // within board
	bitarray <<= 1
</pre>

But the above code only works when all the ones of the bitarray are located in
one row! When multiple rows overflow across the edge when left-shifting we have
to perform the above operation separately for each row of the bitboard and OR
all the results together.

Example:

<pre>
        0 0 0 0 1
        0 0 0 0 0
board = 0 0 1 0 0
        0 0 0 0 0
        1 1 1 0 1

RSHIFT(board)  = SAFE_RSHIFT(row1) | SAFE_RSHIFT(row2) | .. | SAFE_RSHIFT(row5)
RSHIFT(board) &= board_mask // zero the outliers
</pre>
where,

<pre>
       0 0 0 0 0
       0 0 0 0 0
row1 = 0 0 0 0 0
       0 0 0 0 0
       1 1 1 0 1
</pre>

and,
<pre>
                    0 0 0 0 0
                    0 0 0 0 0
SAFE_RSHIFT(row1) = 0 0 0 0 0
                    1 0 0 0 0
                    0 1 1 1 0
</pre>
etc..

Finally,
<pre>

                0 0 0 0 0
                0 0 0 0 0
RSHIFT(board) = 0 0 0 1 0
                1 0 0 0 0
                0 1 1 1 0
</pre>

### Search Algorithm
A\* or IDA\* with consistent heuristics
