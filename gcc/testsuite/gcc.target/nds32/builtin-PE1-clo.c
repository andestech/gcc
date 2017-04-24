/* This is a test program for clo instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int c = 0xFFFF0000;
  c =  __nds32__clo (c);

  if (c != 16)
    abort ();
  else
    exit (0);
}
