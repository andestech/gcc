/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kslraw.u" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;

  r = __nds32__kslraw_u (a, b);
}
