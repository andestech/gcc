/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16 -std=gnu99" } */

#include <math.h>

/* 0x7c00 = 31744 */
__fp16 xx = INFINITY;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t31744" } } */
