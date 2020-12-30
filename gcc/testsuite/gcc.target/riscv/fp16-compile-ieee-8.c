/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

/* Encoding taken from:  http://en.wikipedia.org/wiki/Half_precision */
/* This is the minimum normalized value.  */
/* 0x0400 = 1024 */
__fp16 xx = 6.10352E-5;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t1024" } } */
