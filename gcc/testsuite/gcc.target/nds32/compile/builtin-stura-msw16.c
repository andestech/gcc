/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmmawb" } } */
/* { dg-final { scan-assembler "kmmawb" } } */
/* { dg-final { scan-assembler "kmmawb.u" } } */
/* { dg-final { scan-assembler "kmmawb.u" } } */
/* { dg-final { scan-assembler "kmmawt" } } */
/* { dg-final { scan-assembler "kmmawt" } } */
/* { dg-final { scan-assembler "kmmawt.u" } } */
/* { dg-final { scan-assembler "kmmawt.u" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;
  int16x2_t vb;

  r = __nds32__kmmawb (r, a, b);
  r = __nds32__v_kmmawb (r, a, vb);

  r = __nds32__kmmawb_u (r, a, b);
  r = __nds32__v_kmmawb_u (r, a, vb);

  r = __nds32__kmmawt (r, a, b);
  r = __nds32__v_kmmawt (r, a, vb);

  r = __nds32__kmmawt_u (r, a, b);
  r = __nds32__v_kmmawt_u (r, a, vb);
}
