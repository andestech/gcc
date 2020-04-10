/* This is a test program for bpick instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long bpick (unsigned long ra, unsigned long rb, unsigned long rc)
{
  return __nds__bpick (ra, rb, rc);
}

int
main ()
{
  unsigned long a = bpick (0x11223344, 0x11332244, 0);

  if (a != 0x11332244)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
