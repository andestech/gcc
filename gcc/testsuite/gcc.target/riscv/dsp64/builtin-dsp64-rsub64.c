/* This is a test program for rsub64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long long rsub64 (long long ra, long long rb)
{
  return __nds__rsub64 (ra, rb);
}

int
main ()
{
  long long a = rsub64 (0xe, 0xf);

  if (a != 0xffffffffffffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
