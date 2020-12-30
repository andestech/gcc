/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmmawb" } } */
/* { dg-final { scan-assembler "kmmawb" } } */
/* { dg-final { scan-assembler "kmmawb.u" } } */
/* { dg-final { scan-assembler "kmmawb.u" } } */
/* { dg-final { scan-assembler "kmmawt" } } */
/* { dg-final { scan-assembler "kmmawt" } } */
/* { dg-final { scan-assembler "kmmawt.u" } } */
/* { dg-final { scan-assembler "kmmawt.u" } } */

#include <nds_intrinsic.h>

void
test (void)
{
  int r, a;
  unsigned int b;
  int16x2_t vb;

  r = __nds__kmmawb (r, a, b);
  r = __nds__v_kmmawb (r, a, vb);

  r = __nds__kmmawb_u (r, a, b);
  r = __nds__v_kmmawb_u (r, a, vb);

  r = __nds__kmmawt (r, a, b);
  r = __nds__v_kmmawt (r, a, vb);

  r = __nds__kmmawt_u (r, a, b);
  r = __nds__v_kmmawt_u (r, a, vb);
}
