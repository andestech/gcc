/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh -O2" } */

_Float16 fnmadd(_Float16 a, _Float16 b, _Float16 c) {
  return - a * b - c;
}

/* { dg-final { scan-assembler "fnmadd.h" } } */
