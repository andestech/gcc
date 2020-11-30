/* Test that __bf16 is supported as a parameter type.  */
/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mbf16" } */
extern void f (__bf16);
extern void (*pf) (__bf16);

extern void g (__bf16 *);
extern void (*pg) (__bf16 *);
