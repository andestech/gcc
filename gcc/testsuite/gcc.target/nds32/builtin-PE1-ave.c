/* This is a test program for ave instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int a = 4;
  int b = 2;
  int ave = __nds32__ave (a, b);

  if (ave != 3)
    abort ();
  else
    exit (0);
}
