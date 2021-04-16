/* This is a test program for bsp instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf2 } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0x0000000F;
  unsigned int b = 0x00000300;
  unsigned int r = 0;
  unsigned int verify_b = 0x00000300;
  unsigned int verify_r = 0;

  __nds32__bsp (&r, a, &b);
  asm volatile ("bsp %0, %2, %1": "+&r" (verify_r), "+&r" (verify_b) : "r" (a));

  if ((verify_b == b) && (verify_r == r))
    exit (0);
  else
    abort ();
}
