/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

/* Encoding taken from:  http://en.wikipedia.org/wiki/Half_precision */
/* 0xc000 = 49152 */
__fp16 xx = -2.0;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t49152" } } */
