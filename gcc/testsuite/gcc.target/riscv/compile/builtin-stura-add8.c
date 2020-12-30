/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kadd8" } } */
/* { dg-final { scan-assembler "kadd8" } } */
/* { dg-final { scan-assembler "ukadd8" } } */
/* { dg-final { scan-assembler "ukadd8" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va, vb;
  uint8x4_t v_ur, v_ua, v_ub;

  r = __nds__kadd8 (a, b);
  vr = __nds__v_kadd8 (va, vb);

  r = __nds__ukadd8 (a, b);
  v_ur = __nds__v_ukadd8 (v_ua, v_ub);
}
