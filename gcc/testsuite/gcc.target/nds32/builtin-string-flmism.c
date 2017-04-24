/* This is a test program for flmism instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_string } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x1b2a3d4c;
  unsigned int b = 0x112a334c;
  int r;

  r = __nds32__flmism (a, b);

#ifdef __NDS32_EL__
  if (r != -1)
    abort ();
#else
  if (r != -2)
    abort ();
#endif

  exit (0);
}
