/* This is a test program for bitrev instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long bitrev (unsigned long ra, unsigned long rb)
{
  return __nds__bitrev (ra, rb);
}

int
main ()
{
  unsigned long a = bitrev (0xd, 1);

  if (a != 0x2)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
