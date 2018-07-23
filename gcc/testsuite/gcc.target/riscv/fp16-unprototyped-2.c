/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

extern int f ();

int
f (double xx, double yy)
{
  if (xx == 42.0 && yy == -42.0)
    return 1;
  return 0;
}
