/* This is a test program for clip instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int c = 33;
  c = __nds32__clip (c, 5);

  if (c != 31)
    abort ();
  else
    exit (0);
}
