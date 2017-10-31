/* This is a test program for btst instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int c = 1;
  c = __nds32__btst (c, 0);

  if (c != 1)
    abort ();
  else
    exit (0);
}
