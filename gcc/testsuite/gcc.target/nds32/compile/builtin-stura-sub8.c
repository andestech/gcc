/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksub8" } } */
/* { dg-final { scan-assembler "ksub8" } } */
/* { dg-final { scan-assembler "uksub8" } } */
/* { dg-final { scan-assembler "uksub8" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va, vb;
  uint8x4_t v_ur, v_ua, v_ub;

  r = __nds32__ksub8 (a, b);
  vr = __nds32__v_ksub8 (va, vb);

  r = __nds32__uksub8 (a, b);
  v_ur = __nds32__v_uksub8 (v_ua, v_ub);
}
