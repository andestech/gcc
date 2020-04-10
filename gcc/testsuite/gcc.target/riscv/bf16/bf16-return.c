/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mbf16" } */

/* Test that __bf16 is supported as a return type.  */
extern __bf16 f (void);
extern __bf16 (*pf) (void);

extern __bf16 *g (void);
extern __bf16 *(*pg) (void);
