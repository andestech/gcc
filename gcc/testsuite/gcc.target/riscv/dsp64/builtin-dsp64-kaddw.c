/* This is a test program for kaddw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int kaddw (int ra, int rb)
{
  return __nds__kaddw (ra, rb);
}

int
main ()
{
  int sa = kaddw (0x11223344, 0x55667788);

  if (sa != 0x6688aacc)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
