/* { dg-do compile } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned byteswap(unsigned x) {
    x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
    x = (x & 0x00FF00FF) << 8 | (x & 0xFF00FF00) >> 8;
    return x;
}

/* { dg-final { scan-assembler-times "swap8" 1 } } */
/* { dg-final { scan-assembler-times "swap16" 1 } } */
