/* This is a test program for raddw instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int raddw (int ra, int rb)
{
  return __nds32__raddw (ra, rb);
}

int
main ()
{
  int a = raddw (0x80000000, 0x80000000);

  if (a != 0x80000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
