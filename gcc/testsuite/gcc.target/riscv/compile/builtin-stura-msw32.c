/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmmac" } } */
/* { dg-final { scan-assembler "kmmac.u" } } */
/* { dg-final { scan-assembler "kmmsb" } } */
/* { dg-final { scan-assembler "kmmsb.u" } } */
/* { dg-final { scan-assembler "kwmmul" } } */
/* { dg-final { scan-assembler "kwmmul.u" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r, a, b;
  r = __nds__kmmac (r, a, b);
  r = __nds__kmmac_u (r, a, b);

  r = __nds__kmmsb (r, a, b);
  r = __nds__kmmsb_u (r, a, b);

  r = __nds__kwmmul (a, b);
  r = __nds__kwmmul_u (a, b);
}
