/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksll" } } */
/* { dg-final { scan-assembler "kslli" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;

  r = __nds32__ksll (a, b);
  r = __nds32__ksll (a, 0);
}
