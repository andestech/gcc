/* This is a test program for smsr64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smsr64 (long long rd, long ra, long rb)
{
  return __nds__smsr64 (rd, ra, rb);
}

int
main ()
{
  long long a = smsr64 (0x1, 0xffff0000ffff0000ll, 0x0001ffff0001ffffll);

  if (a != 0x3fffe0001ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
