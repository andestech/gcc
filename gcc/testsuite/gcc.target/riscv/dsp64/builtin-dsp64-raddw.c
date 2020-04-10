/* This is a test program for raddw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long raddw (long ra, long rb)
{
  return __nds__raddw (ra, rb);
}

int
main ()
{
  long a = raddw (0x80000000, 0x80000000);

  if (a != 0xffffffff80000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
