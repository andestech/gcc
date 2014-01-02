/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksll16" } } */
/* { dg-final { scan-assembler "ksll16" } } */
/* { dg-final { scan-assembler "kslli16" } } */
/* { dg-final { scan-assembler "kslli16" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va;

  r = __nds32__ksll16 (a, b);
  vr = __nds32__v_ksll16 (va, b);

  r = __nds32__ksll16 (a, 0);
  vr = __nds32__v_ksll16 (va, 0);
}
