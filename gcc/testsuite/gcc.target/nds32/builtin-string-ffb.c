/* This is a test program for ffb instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_string } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x1b2a3d4c;
  unsigned int b = 0x0000003d;
  int r;

  r =  __nds32__ffb (a, b);

#ifdef __NDS32_EL__
  if (r != -3)
    abort ();
#else
  if (r != -2)
    abort ();
#endif

  exit (0);
}
