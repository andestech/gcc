/* This is a test program for bse instruction.  */

/* { dg-do run } */
/* { dg-options "-O1" } */
/* { dg-require-effective-target nds32_ext_perf2 } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned int a = 0xF0F0F0F0;
  unsigned int b = 0x00000300;
  unsigned int r = 0;

  unsigned int verify_b = 0x00000300;
  unsigned int verify_r = 0;

  __nds32__bse (&r, a, &b);
  a = 0xF0F0F0F0;
  asm volatile ("bse %0, %2, %1": "+&r" (verify_r), "+&r" (verify_b) : "r" (a));

  if ((verify_b == b) && (verify_r == r))
    exit (0);
  else
    abort ();
}
