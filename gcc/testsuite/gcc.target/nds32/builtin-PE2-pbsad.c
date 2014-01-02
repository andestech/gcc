/* This is a test program for pbsad instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf2 } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x09070605;
  unsigned int b = 0x04020301;
  unsigned int r = __nds32__pbsad (a, b);

  if (r != 17)
    abort ();
  else
    exit (0);
}
