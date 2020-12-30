/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

/* Encoding taken from:  http://en.wikipedia.org/wiki/Half_precision */
/* This is the minimum denormalized value.  */
/* 0x0001 = 1 */
__fp16 xx = 5.96046E-8;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t1" } } */
