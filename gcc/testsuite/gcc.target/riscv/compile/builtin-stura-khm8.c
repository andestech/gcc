/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "khm8" } } */
/* { dg-final { scan-assembler "khm8" } } */
/* { dg-final { scan-assembler "khmx8" } } */
/* { dg-final { scan-assembler "khmx8" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va, vb;

  r = __nds__khm8 (a, b);
  vr = __nds__v_khm8 (va, vb);

  r = __nds__khmx8 (a, b);
  vr = __nds__v_khmx8 (va, vb);
}
