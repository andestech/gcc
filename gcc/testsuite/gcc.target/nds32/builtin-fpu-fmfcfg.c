/* This is a test program for fmfcfg instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_fpu } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int intrinsic_fmfcfg = -1;
  unsigned int inline_assemble_fmfcfg = -2;

  intrinsic_fmfcfg = __nds32__fmfcfg ();
  __asm volatile ("fmfcfg %0" : "=r" (inline_assemble_fmfcfg));

  if (intrinsic_fmfcfg == inline_assemble_fmfcfg)
    exit (0);
  else
    abort ();
}
