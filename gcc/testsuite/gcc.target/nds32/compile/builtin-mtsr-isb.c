/* Verify that we generate mtsr and isb instruction with builtin function.  */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler "\\tmtsr" } } */
/* { dg-final { scan-assembler "\\tisb" } } */

#include <nds32_intrinsic.h>

void
main (void)
{
  __nds32__mtsr_isb (1, NDS32_SR_ILMB);
}
