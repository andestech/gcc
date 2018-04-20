/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kslra16" } } */
/* { dg-final { scan-assembler "kslra16" } } */
/* { dg-final { scan-assembler "kslra16.u" } } */
/* { dg-final { scan-assembler "kslra16.u" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va;

  r = __nds__kslra16 (a, b);
  vr = __nds__v_kslra16 (va, b);

  r = __nds__kslra16_u (a, 0);
  vr = __nds__v_kslra16_u (va, 0);
}
