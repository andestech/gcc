/* Verify the return address with builtin function.  */

/* { dg-do run } */
/* { dg-options "-O1" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int main()
{
  unsigned int intrinsic_lp = -1;
  unsigned int inline_assemble_lp = -2;

  intrinsic_lp = __nds32__return_address ();

  __asm volatile ("mov55 %0, $lp" : "=r" (inline_assemble_lp));

  if (intrinsic_lp != inline_assemble_lp)
    abort ();
  else
    exit (0);
}
