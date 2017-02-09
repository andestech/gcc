/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kcrsa16" } } */
/* { dg-final { scan-assembler "kcrsa16" } } */
/* { dg-final { scan-assembler "ukcrsa16" } } */
/* { dg-final { scan-assembler "ukcrsa16" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned long r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds__kcrsa16 (a, b);
  vr = __nds__v_kcrsa16 (va, vb);

  r = __nds__ukcrsa16 (a, b);
  v_ur = __nds__v_ukcrsa16 (v_ua, v_ub);
}
