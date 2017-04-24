/* This is a test program for wsbh instruction.  */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x03020100;
  unsigned int b;

  b = __nds32__wsbh (a);

  if (b != 0x02030001)
    abort ();
  else
    exit (0);
}
