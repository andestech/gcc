/* { dg-do compile } */
/* { dg-options "-O3 -mext-dsp" } */

unsigned extract_word(const unsigned src) {
    return (src >> 13) | (src << (32 - 13));
}

/* { dg-final { scan-assembler-times "wexti" 1 } } */
