/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

int fcvt_w_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.w.h" } } */

unsigned int fcvt_wu_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.wu.h" } } */

_Float16 fcvt_h_w(int a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.w" } } */

_Float16 fcvt_h_wu(unsigned int a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.wu" } } */
