/* This is a test program for abs instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int a = -4;
  int abs = __nds32__abs (a);

  if (abs != 4)
    abort ();
  else
    exit (0);
}
