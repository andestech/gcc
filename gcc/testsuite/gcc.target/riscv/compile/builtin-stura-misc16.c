/* { dg-do compile } */
/* { dg-require-effective-target riscv32 } */
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

#include <nds_intrinsic.h>

void
test (void)
{
  unsigned int r, a, b;
  int16x2_t vr, va, vb;

  r = __nds__sclip16 (a, 0);
  vr = __nds__v_sclip16 (va, 0);

  r = __nds__uclip16 (a, 0);
  vr = __nds__v_uclip16 (va, 0);

  r = __nds__khm16 (a, b);
  vr = __nds__v_khm16 (va, vb);

  r = __nds__khmx16 (a, b);
  vr = __nds__v_khmx16 (va, vb);

  r = __nds__kabs16 (a);
  vr = __nds__v_kabs16 (va);

}
