/* This is a test program for sra.u instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int srau (int ra, unsigned int rb)
{
  return __nds32__sra_u (ra, rb);
}

int
main ()
{
  int a = srau (0xf00ff, 8);

  if (a != 0xf01)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
