/* This is a test program for wext instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long wext (long long ra, unsigned long rb)
{
  return __nds__wext (ra, rb);
}

int
main ()
{
  unsigned long a = wext (0x1234ffff0000ll, 16);

  if (a != 0x1234ffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
