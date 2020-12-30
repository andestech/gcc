/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mno-fp16" } */

__fp16 xx = 0.0;  /* { dg-error "unknown type name" } */
