/* This is a test program for umsr64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long umsr64 (unsigned long long t, unsigned int a, unsigned int b)
{
  return __nds__umsr64 (t, a, b);
}

int
main ()
{
  unsigned long long a = umsr64 (0x5000000300000000ull, 0x12345678, 0x23);

  if (a != 0x5000000082D82D98ull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
