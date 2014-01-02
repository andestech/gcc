/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpswi" } } */
/* { dg-final { scan-assembler "cpswi.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int *base;
  __nds32__cpswi(1, 3, base, 0x450);
  __nds32__cpswi_bi(1, 3, base, 0x450);
}
