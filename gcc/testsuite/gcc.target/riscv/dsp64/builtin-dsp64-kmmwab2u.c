/* This is a test program for kmmawb2.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kmmawb2u (long rd, long ra, long rb)
{
  return __nds__kmmawb2_u (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmmawb2u (long rd, long ra, int16x4_t rb)
{
  return __nds64__v_kmmawb2_u (rd, ra, rb);
}

int
main ()
{
  long a = kmmawb2u (0, 0x8000000080000000, 0x8000000180000001);
  long v_sa = v_kmmawb2u (0, 0x8000000080000000,
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
