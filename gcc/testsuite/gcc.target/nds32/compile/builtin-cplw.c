/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cplw" } } */
/* { dg-final { scan-assembler "cplw.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int *base;
  unsigned int roffset;
  __nds32__cplw(1, 3, base, roffset, 2);
  __nds32__cplw_bi(1, 3, base, roffset, 2);
}
