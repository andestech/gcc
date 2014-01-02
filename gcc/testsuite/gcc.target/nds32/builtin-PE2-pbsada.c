/* This is a test program for pbsada instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf2 } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x09070605;
  unsigned int b = 0x04020301;
  unsigned int r = 1;

  r = __nds32__pbsada(r, a, b);

  if (r != 18)
    abort ();
  else
    exit (0);
}
