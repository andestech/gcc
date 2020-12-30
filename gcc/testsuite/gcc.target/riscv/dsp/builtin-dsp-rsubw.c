/* This is a test program for rsubw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long rsubw (long ra, long rb)
{
  return __nds__rsubw (ra, rb);
}

int
main ()
{
  long a = rsubw (0x80000000, 0x7fffffff);

  if (a != 0x80000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
