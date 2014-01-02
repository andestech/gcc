/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksub16" } } */
/* { dg-final { scan-assembler "ksub16" } } */
/* { dg-final { scan-assembler "uksub16" } } */
/* { dg-final { scan-assembler "uksub16" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds32__ksub16 (a, b);
  vr = __nds32__v_ksub16 (va, vb);

  r = __nds32__uksub16 (a, b);
  v_ur = __nds32__v_uksub16 (v_ua, v_ub);
}
