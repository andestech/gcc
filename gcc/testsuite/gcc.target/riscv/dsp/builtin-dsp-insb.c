/* This is a test program for insb instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long insb (unsigned long ra, unsigned long rb)
{
  return __nds__insb (ra, rb, 1);
}

int
main ()
{
  unsigned long a = insb (0x11220044, 0x33);

  if (a != 0x11223344)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
