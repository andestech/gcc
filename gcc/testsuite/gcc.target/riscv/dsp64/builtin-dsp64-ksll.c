/* This is a test program for ksll instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int ksll (int ra, unsigned long rb)
{
  return __nds__ksllw (ra, rb);
}

int
main ()
{
  int a = ksll (0xf00ff, 8);

  if (a != 0xf00ff00)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
