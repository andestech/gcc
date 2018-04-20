/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "ksub16" } } */
/* { dg-final { scan-assembler "ksub16" } } */
/* { dg-final { scan-assembler "uksub16" } } */
/* { dg-final { scan-assembler "uksub16" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned long r, a, b;
  int16x2_t vr, va, vb;
  uint16x2_t v_ur, v_ua, v_ub;

  r = __nds__ksub16 (a, b);
  vr = __nds__v_ksub16 (va, vb);

  r = __nds__uksub16 (a, b);
  v_ur = __nds__v_uksub16 (v_ua, v_ub);
}
