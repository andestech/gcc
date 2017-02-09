/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "sclip32" } } */
/* { dg-final { scan-assembler "uclip32" } } */
/* { dg-final { scan-assembler "kabs" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned long ur, ua;
  long r, a;

  r = __nds__sclip32 (a, 4);
  ur = __nds__uclip32 (a, 0);
  r = __nds__kabsw (a);
}
