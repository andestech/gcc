/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kadd64" } } */
/* { dg-final { scan-assembler "ukadd64" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  long long r, a, b;
  unsigned long long ur, ua, ub;

  r = __nds32__kadd64 (a, b);
  ur = __nds32__ukadd64 (ua, ub);

}
