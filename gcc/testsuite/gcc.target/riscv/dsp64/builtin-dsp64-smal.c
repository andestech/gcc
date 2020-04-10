/* This is a test program for smal instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long long smal (long long ra, unsigned long rb)
{
  return __nds__smal (ra, rb);
}

static __attribute__ ((noinline))
long long v_smal (long long ra, int16x4_t rb)
{
  return __nds__v_smal (ra, rb);
}

int
main ()
{
  long long a = smal (0xffff0000ffff0000ll, 0x0001ffff0001ffffll);
  long long va = v_smal (0xffffff00ffffff00ll,
			 (int16x4_t) {0x0002, 0xffff, 0x0002, 0xffff});
  if (a != 0xffff0000fffefffe)
    abort ();
  else if (va != 0xffffff00fffffefc)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
