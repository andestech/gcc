/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksll8" } } */
/* { dg-final { scan-assembler "ksll8" } } */
/* { dg-final { scan-assembler "kslli8" } } */
/* { dg-final { scan-assembler "kslli8" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va;

  r = __nds__ksll8 (a, b);
  vr = __nds__v_ksll8 (va, b);

  r = __nds__ksll8 (a, 0);
  vr = __nds__v_ksll8 (va, 0);
}
