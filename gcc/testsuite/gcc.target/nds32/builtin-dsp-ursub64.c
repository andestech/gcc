/* This is a test program for ursub64 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned long long ursub64 (unsigned long long ra, unsigned long long rb)
{
  return __nds32__ursub64 (ra, rb);
}

int
main ()
{
  unsigned long long a = ursub64 (0xeull, 0xfull);

  if (a != 0xffffffffffffffffull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
