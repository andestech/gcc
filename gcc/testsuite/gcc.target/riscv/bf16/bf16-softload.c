/* Test that BFmode should locate in define_mode_attr softload */
/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-options "-O1 -mbf16" } */

int
foo (void)
{
  union
  {
    __bf16 arr[2];
    int x;
  } u;
  u.arr[0] = 0.8;
  return u.x;
}

/* { dg-final { scan-assembler "lh" } } */
