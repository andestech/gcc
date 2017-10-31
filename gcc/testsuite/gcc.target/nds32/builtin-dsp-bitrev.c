/* This is a test program for bitrev instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int bitrev (unsigned int ra, unsigned int rb)
{
  return __nds32__bitrev (ra, rb);
}

int
main ()
{
  unsigned int a = bitrev (0xd, 1);

  if (a != 0x2)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
