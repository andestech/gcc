/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-options "-mfp16" } */

float xx __attribute__((mode(HF))) = 0.0;

/* { dg-final { scan-assembler "\t.size\txx, 2" } } */
/* { dg-final { scan-assembler "\t.zero\t2" } } */
