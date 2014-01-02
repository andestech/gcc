/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kadd16" } } */
/* { dg-final { scan-assembler "kadd16" } } */
/* { dg-final { scan-assembler "ukadd16" } } */
/* { dg-final { scan-assembler "ukadd16" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds32__kadd16 (a, b);
  vr = __nds32__v_kadd16 (va, vb);

  r = __nds32__ukadd16 (a, b);
  v_ur = __nds32__v_ukadd16 (v_ua, v_ub);
}
