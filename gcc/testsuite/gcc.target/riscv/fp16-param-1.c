/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

/* Test that __fp16 is supported as a parameter type.  */
extern void f (__fp16);
extern void (*pf) (__fp16);

extern void g (__fp16 *);
extern void (*pg) (__fp16 *);
