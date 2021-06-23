/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

__fp16
test2 (float x)
{
  return (__fp16) x;
}

/* { dg-final { scan-assembler "fshw" } } */
