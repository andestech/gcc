/* { dg-do compile } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

/* Test that __fp16 is supported as a return type.  */
extern __fp16 f (void);
extern __fp16 (*pf) (void);

extern __fp16 *g (void);
extern __fp16 *(*pg) (void);
