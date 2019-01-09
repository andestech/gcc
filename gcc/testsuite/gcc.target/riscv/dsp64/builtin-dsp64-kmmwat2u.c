/* This is a test program for kmmawt2.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmmawt2u (long rd, long ra, long rb)
{
  return __nds__kmmawt2_u (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmmawt2u (long rd, long ra, int16x4_t rb)
{
  return __nds__v_kmmawt2_u (rd, ra, rb);
}

int
main ()
{
  long a = kmmawt2u (0, 0x8000000080000000, 0x8000000180000001);
  long v_sa = v_kmmawt2u (0, 0x8000000080000000,
			  (int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0x7fffffff7fffffff)
    abort ();
  else if (v_sa != 0x7fffffff7fffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
