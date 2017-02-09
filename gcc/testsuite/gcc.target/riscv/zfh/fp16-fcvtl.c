/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

long long fcvt_l_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.l.h" } } */

unsigned long long fcvt_lu_h(_Float16 a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.lu.h" } } */

_Float16 fcvt_h_l(long long a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.l" } } */

_Float16 fcvt_h_lu(unsigned long long a) {
  return a;
}

/* { dg-final { scan-assembler "fcvt.h.lu" } } */
