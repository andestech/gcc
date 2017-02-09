/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksub64" } } */
/* { dg-final { scan-assembler "uksub64" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  long long r, a, b;
  unsigned long long ur, ua, ub;

  r = __nds__ksub64 (a, b);
  ur = __nds__uksub64 (ua, ub);

}
