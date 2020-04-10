/* This is a test program for uksub64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long uksub64 (unsigned long long ra, unsigned long long rb)
{
  return __nds__uksub64 (ra, rb);
}

int
main ()
{
  unsigned long long sa = uksub64 (0x1122334400000000ll, 0x55667788ll);

  if (sa != 0x11223343aa998878ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
