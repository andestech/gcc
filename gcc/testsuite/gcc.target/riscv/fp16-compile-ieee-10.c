/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16 -std=gnu99" } */

#include <math.h>

/* 0x7e00 = 32256 */
__fp16 xx = NAN;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t32256" } } */
