/* Verify that we generate isb instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tisb" } } */

#include <nds32_intrinsic.h>

void
test (void)
{
  __nds32__isb ();
}
