/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mno-fp16" } */

/* mode(HF) attributes are not recognized unless you explicitly enable
   half-precision floating point.  */
float xx __attribute__((mode(HF))) = 0.0;  /* { dg-error "HF" } */
