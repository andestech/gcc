/* This is a test program for umar64 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long umar64 (unsigned long long t,unsigned int a,unsigned int b)
{
  return __nds32__umar64 (t, a, b);
}

int
main ()
{
  unsigned long long a = umar64 (0xf000000000000000ull, 0x12345678, 0x23);

  if (a != 0xf00000027d27d268ull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
