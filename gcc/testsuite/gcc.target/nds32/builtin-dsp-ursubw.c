/* This is a test program for ursubw instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int ursubw (unsigned int ra,unsigned int rb)
{
  return __nds32__ursubw (ra, rb);
}

int
main ()
{
  unsigned int a = ursubw (0x80000000, 0x40000000);

  if (a != 0x20000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
