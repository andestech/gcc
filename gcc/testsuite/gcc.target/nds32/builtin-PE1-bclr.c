/* This is a test program for bclr instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int a = 1;
  int c = __nds32__bclr (a, 0);

  if (c != 0)
    abort ();
  else
    exit (0);
}
