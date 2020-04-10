/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

float
test (__fp16 x)
{
  return (float) x;
}

/* { dg-final { scan-assembler "flhw" } } */
