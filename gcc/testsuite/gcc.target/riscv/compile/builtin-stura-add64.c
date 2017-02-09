/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kadd64" } } */
/* { dg-final { scan-assembler "ukadd64" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  long long r, a, b;
  unsigned long long ur, ua, ub;

  r = __nds__kadd64 (a, b);
  ur = __nds__ukadd64 (ua, ub);

}
