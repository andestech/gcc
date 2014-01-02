/* This is a test program for btgl instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int a = 1;
  int c = __nds32__btgl (1, 0);

  if (c != 0)
    abort ();
  else
    exit (0);
}
