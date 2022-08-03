/* This is a test program for add64 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long long sadd64 (long long ra, long long rb)
{
  return __nds__sadd64 (ra, rb);
}

static __attribute__ ((noinline))
unsigned long long uadd64 (unsigned long long ra, unsigned long long rb)
{
  return __nds__uadd64 (ra, rb);
}

int
main ()
{
  long long sa = sadd64 (0x1122334400000000ll, 0x55667788ll);
  unsigned long long ua = uadd64 (0xffff00000000ull, 0x55667788ull);

  if (sa != 0x1122334455667788ll)
    abort ();
  else if (ua != 0xffff55667788ull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
