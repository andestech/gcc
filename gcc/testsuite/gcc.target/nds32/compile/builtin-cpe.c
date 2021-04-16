/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "cpe1" } } */
/* { dg-final { scan-assembler "cpe2" } } */
/* { dg-final { scan-assembler "cpe3" } } */
/* { dg-final { scan-assembler "cpe4" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  __nds32__cpe1 (0, 0x7ffff);
  __nds32__cpe2 (3, 0x7bcde);
  __nds32__cpe3 (2, 0x7bcde);
  __nds32__cpe4 (1, 0x7bcde);
}
