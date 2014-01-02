/* This is a test program for fmtcsr/fmfcsr instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_fpu } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int fpcsr;
  unsigned int real_fpcsr;

  /* Keep real fpcsr value.  */
  real_fpcsr = __nds32__fmfcsr ();

  /* write fpcsr */
  fpcsr = 3;
  __nds32__fmtcsr (fpcsr);

  /* read fpcsr */
  fpcsr = 0;
  fpcsr = __nds32__fmfcsr ();
  fpcsr = fpcsr & 0x00001fff;

  /* Recover fpcsr value.  */
  __nds32__fmtcsr (real_fpcsr);

  if (fpcsr == 3)
    exit (0);
  else
   abort ();
}
