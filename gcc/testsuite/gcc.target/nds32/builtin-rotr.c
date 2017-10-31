/* This is a test program for rotr instruction.  */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 1;
  a = __nds32__rotr (a, 30);

  if (a != 4)
    abort ();
  else
    exit (0);
}
