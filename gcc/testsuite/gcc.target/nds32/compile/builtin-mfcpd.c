/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "mfcpd" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned long long data64;
  data64 = __nds32__mfcpd(1, 10);
}
