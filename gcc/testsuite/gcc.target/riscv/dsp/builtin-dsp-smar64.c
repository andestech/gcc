/* This is a test program for smar64 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long long smar64 (long long t, int a, int b)
{
  return __nds__smar64 (t, a, b);
}

int
main ()
{
  long long a = smar64 (0xf000000000000000ll, 0x12345678, 0x23);

  if (a != 0xf00000027d27d268ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
