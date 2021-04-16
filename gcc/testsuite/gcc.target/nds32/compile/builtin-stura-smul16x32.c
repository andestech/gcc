/* { dg-do compile } */
/* { dg-options "-mext-dsp" } */
/* { dg-final { scan-assembler "kmda" } } */
/* { dg-final { scan-assembler "kmda" } } */
/* { dg-final { scan-assembler "kmxda" } } */
/* { dg-final { scan-assembler "kmxda" } } */
/* { dg-final { scan-assembler "kmabb" } } */
/* { dg-final { scan-assembler "kmabb" } } */
/* { dg-final { scan-assembler "kmabt" } } */
/* { dg-final { scan-assembler "kmabt" } } */
/* { dg-final { scan-assembler "kmatt" } } */
/* { dg-final { scan-assembler "kmatt" } } */
/* { dg-final { scan-assembler "kmada" } } */
/* { dg-final { scan-assembler "kmada" } } */
/* { dg-final { scan-assembler "kmaxda" } } */
/* { dg-final { scan-assembler "kmaxda" } } */
/* { dg-final { scan-assembler "kmads" } } */
/* { dg-final { scan-assembler "kmads" } } */
/* { dg-final { scan-assembler "kmadrs" } } */
/* { dg-final { scan-assembler "kmadrs" } } */
/* { dg-final { scan-assembler "kmaxds" } } */
/* { dg-final { scan-assembler "kmaxds" } } */
/* { dg-final { scan-assembler "kmsda" } } */
/* { dg-final { scan-assembler "kmsda" } } */
/* { dg-final { scan-assembler "kmsxda" } } */
/* { dg-final { scan-assembler "kmsxda" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int r;
  unsigned int a, b;
  int16x2_t va, vb;

  r = __nds32__kmda (a, b);
  r = __nds32__v_kmda (va, vb);

  r = __nds32__kmxda (a, b);
  r = __nds32__v_kmxda (va, vb);

  r = __nds32__kmabb (r, a, b);
  r = __nds32__v_kmabb (r, va, vb);

  r = __nds32__kmabt (r, a, b);
  r = __nds32__v_kmabt (r, va, vb);

  r = __nds32__kmatt (r, a, b);
  r = __nds32__v_kmatt (r, va, vb);

  r = __nds32__kmada (r, a, b);
  r = __nds32__v_kmada (r, va, vb);

  r = __nds32__kmaxda (r, a, b);
  r = __nds32__v_kmaxda (r, va, vb);

  r = __nds32__kmads (r, a, b);
  r = __nds32__v_kmads (r, va, vb);

  r = __nds32__kmadrs (r, a, b);
  r = __nds32__v_kmadrs (r, va, vb);

  r = __nds32__kmaxds (r, a, b);
  r = __nds32__v_kmaxds (r, va, vb);

  r = __nds32__kmsda (r, a, b);
  r = __nds32__v_kmsda (r, va, vb);

  r = __nds32__kmsxda (r, a, b);
  r = __nds32__v_kmsxda (r, va, vb);
}
