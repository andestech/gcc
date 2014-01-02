/* This is a test program for fcpysd instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_fpu_dp } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  double da = -1.5;
  double db = 1.3;
  double dr = __nds32__fcpysd (da, db);

  if (dr != 1.5)
    abort ();
  else
    exit (0);
}
