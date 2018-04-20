/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kslra8" } } */
/* { dg-final { scan-assembler "kslra8" } } */
/* { dg-final { scan-assembler "kslra8.u" } } */
/* { dg-final { scan-assembler "kslra8.u" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va;

  r = __nds__kslra8 (a, b);
  vr = __nds__v_kslra8 (va, b);

  r = __nds__kslra8_u (a, 0);
  vr = __nds__v_kslra8_u (va, 0);
}
