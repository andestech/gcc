/* This is a test program for umsr64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long umsr64 (unsigned long long rd, unsigned long ra, unsigned long rb)
{
  return __nds__umsr64 (rd, ra, rb);
}

int
main ()
{
  unsigned long long a = umsr64 (0x1, 0xffff0000ffff0000ll, 0x0001ffff0001ffffll);

  if (a != 0xfffc0005fffe0001ll)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
