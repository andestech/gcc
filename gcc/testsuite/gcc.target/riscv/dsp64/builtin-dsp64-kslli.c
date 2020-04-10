/* This is a test program for kslli instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int kslli (int ra)
{
  return __nds__ksllw (ra, 8);
}

int
main ()
{
  int a = kslli (0xf00ff);

  if (a != 0xf00ff00)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
