/* This is a test program for insb instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int insb (unsigned int ra, unsigned int rb)
{
  return __nds32__insb (ra, rb, 1);
}

int
main ()
{
  unsigned int a = insb (0x11220044, 0x33);

  if (a != 0x11223344)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
