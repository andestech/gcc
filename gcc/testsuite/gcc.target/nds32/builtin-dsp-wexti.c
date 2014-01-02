/* This is a test program for wexti instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int wexti (long long ra)
{
  return __nds32__wext (ra, 16);
}

int
main ()
{
  unsigned int a = wexti (0x1234ffff0000ll);

  if (a != 0x1234ffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
