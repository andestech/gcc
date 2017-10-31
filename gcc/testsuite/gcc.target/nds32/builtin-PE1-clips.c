/* This is a test program for clips instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int a = -33;
  int c = __nds32__clips (a, 5);

  if (c != -32)
    abort ();
  else
    exit (0);
}
