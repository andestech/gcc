/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "mtcppw" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  unsigned int data32;
  __nds32__mtcppw(1, data32, 10);
}
