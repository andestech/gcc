/* This is a test program for ukmsr64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long ukmsr64 (unsigned long long rd, unsigned long ra, unsigned long rb)
{
  return __nds__ukmsr64 (rd, ra, rb);
}

int
main ()
{
  unsigned long long a = ukmsr64 (0x1, 0xffff0000ffff0000ll, 0x0001ffff0001ffffll);

  if (a != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
