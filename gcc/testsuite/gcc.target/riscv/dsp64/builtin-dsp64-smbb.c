/* This is a test program for smbb instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long smbb (unsigned long ra, unsigned long rb)
{
  return __nds__smbb16 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_smbb (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_smbb16 (ra, rb);
}

int
main ()
{
  long a = smbb (0x8000000280000002, 0x8000000180000001);
  int32x2_t va = v_smbb ((int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
			 (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0x200000002)
    abort ();
  else if (va[0] != 0x1
	   || va[1] != 0x1)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
