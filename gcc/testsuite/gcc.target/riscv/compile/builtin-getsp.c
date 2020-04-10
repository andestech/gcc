/* Verify that the content of the stack pointer register can be got by builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O1" } */
/* { dg-final { scan-assembler "\tmv\ta0,\\s*(x2|sp)" } } */

#include <nds_intrinsic.h>

unsigned long
func (void)
{
  return __nds__get_current_sp ();
}
