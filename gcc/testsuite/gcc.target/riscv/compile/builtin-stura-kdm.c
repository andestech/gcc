/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kdmbb" } } */
/* { dg-final { scan-assembler "kdmbb" } } */
/* { dg-final { scan-assembler "kdmbt" } } */
/* { dg-final { scan-assembler "kdmbt" } } */
/* { dg-final { scan-assembler "kdmtt" } } */
/* { dg-final { scan-assembler "kdmtt" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r;
  unsigned int a, b;
  int16x2_t va, vb;

  r = __nds__kdmbb (a, b);
  r = __nds__v_kdmbb (va, vb);

  r = __nds__kdmbt (a, b);
  r = __nds__v_kdmbt (va, vb);

  r = __nds__kdmtt (a, b);
  r = __nds__v_kdmtt (va, vb);
}
