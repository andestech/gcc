/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kcras16" } } */
/* { dg-final { scan-assembler "kcras16" } } */
/* { dg-final { scan-assembler "ukcras16" } } */
/* { dg-final { scan-assembler "ukcras16" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned long r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds__kcras16 (a, b);
  vr = __nds__v_kcras16 (va, vb);

  r = __nds__ukcras16 (a, b);
  v_ur = __nds__v_ukcras16 (v_ua, v_ub);
}
