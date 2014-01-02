/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpld" } } */
/* { dg-final { scan-assembler "cpld.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long *base;
  unsigned int roffset;
  __nds32__cpld(1, 3, base, roffset, 2);
  __nds32__cpld_bi(1, 3, base, roffset, 2);
}
