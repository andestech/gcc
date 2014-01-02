/* This is a test program for wext instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int wext (long long ra, unsigned int rb)
{
  return __nds32__wext (ra, rb);
}

int
main ()
{
  unsigned int a = wext (0x1234ffff0000ll, 16);

  if (a != 0x1234ffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
