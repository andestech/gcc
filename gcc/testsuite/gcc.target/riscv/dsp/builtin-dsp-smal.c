/* This is a test program for smal instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smal (long long ra, unsigned long rb)
{
  return __nds__smal (ra, rb);
}

static __attribute__ ((noinline))
long long v_smal (long long ra, int16x2_t rb)
{
  return __nds__v_smal (ra, rb);
}

int
main ()
{
  long long a = smal (0xfffff0000ll, 0x0001ffff);
  long long va = v_smal (0xffffff0000ll,
			 (int16x2_t) {0x0002, 0xffff});
  if (a != 0xffffeffffll)
    abort ();
  else if (va != 0xfffffefffell)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
