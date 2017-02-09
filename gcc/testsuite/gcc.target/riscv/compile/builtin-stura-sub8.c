/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-require-effective-target riscv32 } */
/* { dg-final { scan-assembler "ksub8" } } */
/* { dg-final { scan-assembler "ksub8" } } */
/* { dg-final { scan-assembler "uksub8" } } */
/* { dg-final { scan-assembler "uksub8" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int8x4_t vr, va, vb;
  uint8x4_t v_ur, v_ua, v_ub;

  r = __nds__ksub8 (a, b);
  vr = __nds__v_ksub8 (va, vb);

  r = __nds__uksub8 (a, b);
  v_ur = __nds__v_uksub8 (v_ua, v_ub);
}
