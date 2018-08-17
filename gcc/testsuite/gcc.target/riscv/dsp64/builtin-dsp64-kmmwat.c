/* This is a test program for kmmawt instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kmmawt (long rd, long ra, long rb)
{
  return __nds__kmmawt (rd, ra, rb);
}

static __attribute__ ((noinline))
long v_kmmawt (long rd, long ra, int16x4_t rb)
{
  return __nds64__v_kmmawt (rd, ra, rb);
}

int
main ()
{
  long a = kmmawt (0, 0x8000000080000000, 0x8000000180000001);
  long v_sa = v_kmmawt (0, 0x8000000080000000,
		       (int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0x4000000040000000)
    abort ();
  else if (v_sa != 0x4000000040000000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
