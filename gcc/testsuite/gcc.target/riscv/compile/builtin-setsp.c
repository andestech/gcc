/* Verify that the content of the stack pointer register can be set by builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O1" } */
/* { dg-final { scan-assembler "\tmv\t(sp|x2),\\s*a0" } } */

#include <nds_intrinsic.h>

void
func (unsigned long sp)
{
  __nds__set_current_sp (sp);
}
