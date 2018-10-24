/* This is a test program for kmmwt2 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kmmwt2 (long ra, long rb)
{
  return __nds__kmmwt2 (ra, rb);
}

static __attribute__ ((noinline))
long v_kmmwt2 (long ra, int16x4_t rb)
{
  return __nds__v_kmmwt2 (ra, rb);
}

int
main ()
{
  long a = kmmwt2 (0x8000000080000000, 0x8000000180000001);
  long v_sa = v_kmmwt2 (0x8000000080000000,
			(int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0x8000000080000000)
    abort ();
  else if (v_sa != 0x8000000080000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
