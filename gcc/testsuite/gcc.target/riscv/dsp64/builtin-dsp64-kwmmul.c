/* This is a test program for kwmmul instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kwmmul (long ra, long rb)
{
  return __nds__kwmmul (ra, rb);
}

int
main ()
{
  long a = kwmmul (0x8000000080000000, 0x8000000080000000);

  if (a != 0x7fffffff7fffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
