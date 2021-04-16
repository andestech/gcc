/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kabs8" } } */
/* { dg-final { scan-assembler "kabs8" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a;
  int8x4_t vr, va;

  r = __nds32__kabs8 (a);
  vr = __nds32__v_kabs8 (va);
}
