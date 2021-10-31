/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

int flt(_Float16 a, _Float16 b) {
  return a < b;
}

/* { dg-final { scan-assembler "flt.h" } } */
