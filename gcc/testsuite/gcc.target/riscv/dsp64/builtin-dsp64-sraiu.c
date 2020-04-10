/* This is a test program for srai.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int sraiu (int ra)
{
  return __nds__sra_u (ra, 8);
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
