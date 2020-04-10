/* This is a test program for radd64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long radd64 (long long ra, long long rb)
{
  return __nds__radd64 (ra, rb);
}

int
main ()
{
  long long a = radd64 (0xf000000000000000ll, 0xf000000000000000ll);

  if (a != 0xf000000000000000ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
