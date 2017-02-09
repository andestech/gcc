/* This is a test program for smsr64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smsr64 (long long t, int a, int b)
{
  return __nds__smsr64 (t, a, b);
}

int
main ()
{
  long long a = smsr64 (0x5000000300000000ll, 0x12345678, 0x23);

  if (a != 0x5000000082D82D98ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
