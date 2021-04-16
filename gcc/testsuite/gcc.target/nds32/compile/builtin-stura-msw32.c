/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmmac" } } */
/* { dg-final { scan-assembler "kmmac.u" } } */
/* { dg-final { scan-assembler "kmmsb" } } */
/* { dg-final { scan-assembler "kmmsb.u" } } */
/* { dg-final { scan-assembler "kwmmul" } } */
/* { dg-final { scan-assembler "kwmmul.u" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int r, a, b;
  r = __nds32__kmmac (r, a, b);
  r = __nds32__kmmac_u (r, a, b);

  r = __nds32__kmmsb (r, a, b);
  r = __nds32__kmmsb_u (r, a, b);

  r = __nds32__kwmmul (a, b);
  r = __nds32__kwmmul_u (a, b);
}
