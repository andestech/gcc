/* This is a test program for uradd64 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long uradd64 (unsigned long long ra, unsigned long long rb)
{
  return __nds32__uradd64 (ra, rb);
}

int
main ()
{
  unsigned long long a = uradd64 (0xf000000000000000ull, 0xf000000000000000ull);

  if (a != 0xf000000000000000ull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
