/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksllw" } } */
/* { dg-final { scan-assembler "kslliw" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;

  r = __nds__ksllw (a, b);
  r = __nds__ksllw (a, 0);
}
