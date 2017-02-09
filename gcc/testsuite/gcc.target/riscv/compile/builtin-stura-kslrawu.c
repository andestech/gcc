/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kslraw.u" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;

  r = __nds__kslraw_u (a, b);
}
