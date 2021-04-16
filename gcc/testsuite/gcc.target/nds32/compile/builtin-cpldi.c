/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpldi" } } */
/* { dg-final { scan-assembler "cpldi.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long *base;
  __nds32__cpldi(1, 3, base, 0x450);
  __nds32__cpldi_bi(1, 3, base, 0x450);
}
