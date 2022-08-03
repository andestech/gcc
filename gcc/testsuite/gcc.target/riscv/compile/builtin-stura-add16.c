/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-require-effective-target riscv32 } */
/* { dg-final { scan-assembler "kadd16" } } */
/* { dg-final { scan-assembler "kadd16" } } */
/* { dg-final { scan-assembler "ukadd16" } } */
/* { dg-final { scan-assembler "ukadd16" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned long r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds__kadd16 (a, b);
  vr = __nds__v_kadd16 (va, vb);

  r = __nds__ukadd16 (a, b);
  v_ur = __nds__v_ukadd16 (v_ua, v_ub);
}
