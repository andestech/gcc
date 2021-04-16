/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "mfcpw" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int data32;
  data32 = __nds32__mfcpw(1, 10);
}
