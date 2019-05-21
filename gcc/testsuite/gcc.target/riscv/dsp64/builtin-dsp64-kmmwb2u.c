/* This is a test program for kmmwb2u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmmwb2u (long ra, long rb)
{
  return __nds__kmmwb2_u (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kmmwb2u (int32x2_t ra, int16x4_t rb)
{
  return __nds__v_kmmwb2_u (ra, rb);
}

int
main ()
{
  long a = kmmwb2u (0x8000000080000000, 0x8000000180000001);
  int32x2_t va = v_kmmwb2u ((int32x2_t) {0x80000000, 0x80000000},
			    (int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (va[0] != 0xffff0000
	   || va[1] != 0xffff0000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
