/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksub64" } } */
/* { dg-final { scan-assembler "uksub64" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  long long r, a, b;
  unsigned long long ur, ua, ub;

  r = __nds32__ksub64 (a, b);
  ur = __nds32__uksub64 (ua, ub);

}
