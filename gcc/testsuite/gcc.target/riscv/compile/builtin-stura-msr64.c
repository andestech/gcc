/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmsr64" } } */
/* { dg-final { scan-assembler "ukmsr64" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  long long r, a, b;
  unsigned long long ur, ua, ub;

  r = __nds__kmsr64 (r, a, b);
  ur = __nds__ukmsr64 (ur, ua, ub);
}
