/* This is a test program for ursubw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ursubw (unsigned int ra, unsigned int rb)
{
  return __nds__ursubw (ra, rb);
}

int
main ()
{
  unsigned long a = ursubw (0x80000000, 0x40000000);

  if (a != 0x20000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
