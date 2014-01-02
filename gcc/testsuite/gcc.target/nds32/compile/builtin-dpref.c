/* Verify that we generate data prefetch instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "dpref\\tSRD" } } */
/* { dg-final { scan-assembler "dpref\\tSRD" } } */
/* { dg-final { scan-assembler "dpref\\tSRD" } } */
/* { dg-final { scan-assembler "dpref\\tSRD" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned char dpref_q = 0;
  unsigned short dpref_h = 0;
  unsigned int dpref_w = 0;
  unsigned long long dpref_dw = 0;

  __nds32__dpref_qw (&dpref_q, 0, NDS32_DPREF_SRD);
  __nds32__dpref_hw (&dpref_h, 0, NDS32_DPREF_SRD);
  __nds32__dpref_w (&dpref_w, 0, NDS32_DPREF_SRD);
  __nds32__dpref_dw (&dpref_dw, 0, NDS32_DPREF_SRD);
}
