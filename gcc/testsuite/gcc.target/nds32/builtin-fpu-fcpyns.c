/* This is a test program for fcpynss instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_fpu_sp } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  float a = -1.5;
  float b = -1.3;
  float r = __nds32__fcpynss (a, b);

  if (r != 1.5)
    abort ();
  else
    exit (0);
}
