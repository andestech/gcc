/* This is a test program for wext instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long wext (long long ra, unsigned long rb)
{
  return __nds__wext (ra, rb);
}

int
main ()
{
  unsigned long a = wext (0x1234ffff0000ll, 16);

  if (a != 0x1234ffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
