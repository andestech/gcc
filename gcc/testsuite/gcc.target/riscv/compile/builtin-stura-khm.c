/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "khmbb" } } */
/* { dg-final { scan-assembler "khmbb" } } */
/* { dg-final { scan-assembler "khmbt" } } */
/* { dg-final { scan-assembler "khmbt" } } */
/* { dg-final { scan-assembler "khmtt" } } */
/* { dg-final { scan-assembler "khmtt" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r;
  unsigned int a, b;
  int16x2_t va, vb;

  r = __nds__khmbb (a, b);
  r = __nds__v_khmbb (va, vb);

  r = __nds__khmbt (a, b);
  r = __nds__v_khmbt (va, vb);

  r = __nds__khmtt (a, b);
  r = __nds__v_khmtt (va, vb);
}
