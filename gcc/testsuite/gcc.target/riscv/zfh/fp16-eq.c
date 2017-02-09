/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */

int feq(_Float16 a, _Float16 b) {
  return a == b;
}

/* { dg-final { scan-assembler "feq.h" } } */
