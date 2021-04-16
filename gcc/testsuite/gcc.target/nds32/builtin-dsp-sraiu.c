/* This is a test program for srai.u instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
int sraiu (int ra)
{
  return __nds32__sra_u (ra, 8);
}

int
main ()
{
  int a = sraiu (0xf00ff);

  if (a != 0xf01)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
