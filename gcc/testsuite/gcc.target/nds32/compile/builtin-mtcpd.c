/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "mtcpd" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long data64;
  __nds32__mtcpd(1, data64, 10);
}
