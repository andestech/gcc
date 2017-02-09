/* This is a test program for kslraw instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
int kslraw (int ra, int rb)
{
  return __nds__kslraw (ra, rb);
}

int
main ()
{
  int a = kslraw (0x0f00f000, 4);

  if (a != 0x7fffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
