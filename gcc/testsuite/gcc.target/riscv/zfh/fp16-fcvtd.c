/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp64 } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

double fcvt_d_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.d.h" } } */

_Float16 fcvt_h_d(double a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.d" } } */
