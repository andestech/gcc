/* This is a test program for clz instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  int c = 0x0000FFFF;
  c =  __nds32__clz (c);

  if (c != 16)
    abort ();
  else
    exit (0);
}
