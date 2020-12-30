/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-O2 -mfp16" } */

/* Test that conversion from 32-bit and 64-bit integers can be done
   without a call to the support library.  */

__fp16
foo (int x)
{
  return x;
}

__fp16
bar (unsigned int x)
{
  return x;
}

__fp16
fool (long long x)
{
  return x;
}

__fp16
barl (unsigned long long x)
{
  return x;
}

