#ifndef __HELPERS_H__
#define __HELPERS_H__

#define BIT(N) (1LLU << (N))
#define BUT(N) (~BIT(N))

#define CHKBIT(A, N) ((A)  & BIT(N))
#define SETBIT(A, N) ((A) |= BIT(N))
#define CLRBIT(A, N) ((A) &= BUT(N))
#define FLPBIT(A, N) ((A) ^= BIT(N))

/* Macros for binary literals
   By Tom Torfs - donated to the public domain
*/
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0)  \
              + ((x&0x000000F0LU)?2:0)  \
              + ((x&0x00000F00LU)?4:0)  \
              + ((x&0x0000F000LU)?8:0)  \
              + ((x&0x000F0000LU)?16:0) \
              + ((x&0x00F00000LU)?32:0) \
              + ((x&0x0F000000LU)?64:0) \
              + ((x&0xF0000000LU)?128:0)

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(d1,d0) (((unsigned short)B8(d1)<<8) \
                  + B8(d0))

/* for upto 32-bit binary constants, MSB first */
#define B32(d3,d2,d1,d0) (((unsigned long long int)B8(d3)<<24) \
                        + ((unsigned long long int)B8(d2)<<16) \
                        + ((unsigned long long int)B8(d1)<<8)  \
                        + B8(d0))

/* for upto 64-bit binary constants, MSB first */
#define B64(d7, d6, d5, d4, d3, d2, d1, d0) (((unsigned long long int)B8(d7)<<56) \
                                           + ((unsigned long long int)B8(d6)<<48) \
                                           + ((unsigned long long int)B8(d5)<<40) \
                                           + ((unsigned long long int)B8(d4)<<32) \
                                           + ((unsigned long long int)B8(d3)<<24) \
                                           + ((unsigned long long int)B8(d2)<<16) \
                                           + ((unsigned long long int)B8(d1)<<8)  \
                                           + B8(d0))

#endif /* __HELPERS_H__ */
