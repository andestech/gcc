/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kadd8" } } */
/* { dg-final { scan-assembler "kadd8" } } */
/* { dg-final { scan-assembler "ukadd8" } } */
/* { dg-final { scan-assembler "ukadd8" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va, vb;
  uint8x4_t v_ur, v_ua, v_ub;

  r = __nds32__kadd8 (a, b);
  vr = __nds32__v_kadd8 (va, vb);

  r = __nds32__ukadd8 (a, b);
  v_ur = __nds32__v_ukadd8 (v_ua, v_ub);
}
