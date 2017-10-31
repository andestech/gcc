/* This is a test program for fcpyss instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_fpu_sp } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  float a = -1.5;
  float b = 1.3;
  float r = __nds32__fcpyss (a, b);

  if (r != 1.5)
    abort ();
  else
    exit (0);
}
