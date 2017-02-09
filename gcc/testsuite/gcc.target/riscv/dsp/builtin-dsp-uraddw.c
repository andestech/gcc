/* This is a test program for uraddw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long uraddw (unsigned long ra, unsigned long rb)
{
  return __nds__uraddw (ra, rb);
}

int
main ()
{
  unsigned long a = uraddw (0x80000000, 0x80000000);

  if (a != 0x80000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
