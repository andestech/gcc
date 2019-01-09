/* This is a test program for kmmacu instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmmacu (long rd, long ra, long rb)
{
  return __nds__kmmac_u (rd, ra, rb);
}

int
main ()
{
  long a = kmmacu (0, 0x8000000080000000, 0x8000000080000000);

  if (a != 0x4000000040000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
