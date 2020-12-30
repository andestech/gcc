/* This is a test program for kmmawb instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmmawb (long rd, long ra, long rb)
{
  return __nds__kmmawb (rd, ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kmmawb (int32x2_t rd, int32x2_t ra, int16x4_t rb)
{
  return __nds__v_kmmawb (rd, ra, rb);
}

int
main ()
{
  long a = kmmawb (0, 0x8000000080000000, 0x8000000180000001);
  int32x2_t va = v_kmmawb ((int32x2_t) {0, 0},
			   (int32x2_t) {0x80000000, 0x80000000},
			   (int16x4_t) {0x1, 0x8000, 0x1, 0x8000});

  if (a != 0xffff8000ffff8000)
    abort ();
  else if (va[0] != 0xffff8000
	   || va[1] != 0xffff8000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
