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
long v_kmmwb2u (long ra, int16x4_t rb)
{
  return __nds__v_kmmwb2_u (ra, rb);
}

int
main ()
{
  long a = kmmwb2u (0x8000000080000000, 0x8000000180000001);
  long v_sa = v_kmmwb2u (0x8000000080000000,
			 (int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0xffff0000ffff0000)
    abort ();
  else if (v_sa != 0xffff0000ffff0000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
