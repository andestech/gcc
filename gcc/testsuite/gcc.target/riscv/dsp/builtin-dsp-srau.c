/* This is a test program for sra.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int srau (int ra, unsigned long rb)
{
  return __nds__sra_u (ra, rb);
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
