/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

float fcvt_s_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.s.h" } } */

_Float16 fcvt_h_s(float a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.s" } } */
