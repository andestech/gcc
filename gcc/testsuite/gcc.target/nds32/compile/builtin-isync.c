/* Verify that we generate isync instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tisync" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  int *addr = (int *) 0x53000000;
  __nds32__isync (addr);
}
