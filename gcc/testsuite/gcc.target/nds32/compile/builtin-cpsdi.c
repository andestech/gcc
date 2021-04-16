/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpsdi" } } */
/* { dg-final { scan-assembler "cpsdi.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long *base;
  __nds32__cpsdi(1, 3, base, 0x450);
  __nds32__cpsdi_bi(1, 3, base, 0x450);
}
