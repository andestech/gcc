/* This is a test program for smmul.u instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int smmul_u (int ra, int rb)
{
  return __nds32__smmul_u (ra, rb);
}

int
main ()
{
  int a = smmul_u (0x80000002, 0x80000001);

  if (a != 0x3fffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
