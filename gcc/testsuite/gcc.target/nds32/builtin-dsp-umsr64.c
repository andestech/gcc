/* This is a test program for umsr64 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long umsr64 (unsigned long long t, unsigned int a, unsigned int b)
{
  return __nds32__umsr64 (t, a, b);
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
