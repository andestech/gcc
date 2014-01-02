/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpsw" } } */
/* { dg-final { scan-assembler "cpsw.bi" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int *base;
  unsigned int roffset;
  __nds32__cpsw(1, 3, base, roffset, 2);
  __nds32__cpsw_bi(1, 3, base, roffset, 2);
}
