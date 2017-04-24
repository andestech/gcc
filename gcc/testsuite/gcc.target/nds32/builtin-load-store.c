/* Verify that we generate llw/lwup/scw/swup instruction
   with builtin function.  */

/* { dg-do compile } */
/* { dg-require-effective-target nds32_no_v3m } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tllw" } } */
/* { dg-final { scan-assembler "\\tlwup" } } */
/* { dg-final { scan-assembler "\\tscw" } } */
/* { dg-final { scan-assembler "\\tswup" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int a = 0;
  int b = 0;
  unsigned int cc = 0;

  __nds32__llw (&a);
  cc = __nds32__lwup (&a);
  __nds32__scw (&a, b);
  __nds32__swup (&a, b);
}
