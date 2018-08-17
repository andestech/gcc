/* This is a test program for ksubw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
int ksubw (int ra, int rb)
{
  return __nds__ksubw (ra, rb);
}

int
main ()
{
  int sa = ksubw (0x11223344, 0x55667788);

  if (sa != 0xbbbbbbbc)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
