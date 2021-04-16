/* This is a test program for bpick instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int bpick (unsigned int ra, unsigned int rb, unsigned int rc)
{
  return __nds32__bpick (ra, rb, rc);
}

int
main ()
{
  unsigned int a = bpick (0x11223344, 0x11332244, 0);

  if (a != 0x11332244)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
