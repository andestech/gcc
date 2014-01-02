/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cplwi" } } */
/* { dg-final { scan-assembler "cplwi.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int *base;
  __nds32__cplwi(1, 3, base, 0x450);
  __nds32__cplwi_bi(1, 3, base, 0x450);
}
