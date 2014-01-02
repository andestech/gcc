/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpsd" } } */
/* { dg-final { scan-assembler "cpsd.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long *base;
  unsigned int roffset;
  __nds32__cpsd(1, 3, base, roffset, 2);
  __nds32__cpsd_bi(1, 3, base, roffset, 2);
}
