/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16 -pedantic" } */

/* Encoding taken from:  http://en.wikipedia.org/wiki/Half_precision */
/* This number is too big and is represented as infinity.  */
/* We should *not* get an overflow warning here.  */
/* 0x7c00 = 31744 */
__fp16 xx = 123456789.0;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.half\t31744" } } */
