/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

/* Test generation of VFP __fp16 instructions.  */

__fp16 h1 = 0.0;
__fp16 h2 = 1234.0;
float f1 = 2.0;
float f2 = -999.9;

void f (void)
{
  h1 = f1;
  f2 = h2;
}
