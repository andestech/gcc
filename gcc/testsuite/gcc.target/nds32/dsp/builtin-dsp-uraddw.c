/* This is a test program for uraddw instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int uraddw (unsigned int ra, unsigned int rb)
{
  return __nds32__uraddw (ra, rb);
}

unsigned int
main ()
{
  unsigned int a = uraddw (0x80000000, 0x80000000);

  if (a != 0x80000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
