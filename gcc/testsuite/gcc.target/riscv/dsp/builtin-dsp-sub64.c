/* This is a test program for sub64 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long long ssub64 (long long ra, long long rb)
{
  return __nds__ssub64 (ra, rb);
}

static __attribute__ ((noinline))
unsigned long long usub64 (unsigned long long ra, unsigned long long rb)
{
  return __nds__usub64 (ra, rb);
}

int
main ()
{
  long long sa = ssub64 (0x100000000ll, 0xffffffffll);
  unsigned long long ua = usub64 (0xf00000000ull, 0x1111ull);

  if (sa != 1ll)
    abort ();
  else if (ua != 0xeffffeeefull)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
