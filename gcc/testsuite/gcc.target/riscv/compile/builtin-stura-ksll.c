/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksll" } } */
/* { dg-final { scan-assembler "kslli" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;

  r = __nds__ksll (a, b);
  r = __nds__ksll (a, 0);
}
