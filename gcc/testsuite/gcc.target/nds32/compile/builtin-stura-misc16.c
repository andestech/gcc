/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "sclip16" } } */
/* { dg-final { scan-assembler "sclip16" } } */
/* { dg-final { scan-assembler "uclip16" } } */
/* { dg-final { scan-assembler "uclip16" } } */
/* { dg-final { scan-assembler "khm16" } } */
/* { dg-final { scan-assembler "khm16" } } */
/* { dg-final { scan-assembler "khmx16" } } */
/* { dg-final { scan-assembler "khmx16" } } */
/* { dg-final { scan-assembler "kabs16" } } */
/* { dg-final { scan-assembler "kabs16" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va, vb;

  r = __nds32__sclip16 (a, 0);
  vr = __nds32__v_sclip16 (va, 0);

  r = __nds32__uclip16 (a, 0);
  vr = __nds32__v_uclip16 (va, 0);

  r = __nds32__khm16 (a, b);
  vr = __nds32__v_khm16 (va, vb);

  r = __nds32__khmx16 (a, b);
  vr = __nds32__v_khmx16 (va, vb);

  r = __nds32__kabs16 (a);
  vr = __nds32__v_kabs16 (va);
}
