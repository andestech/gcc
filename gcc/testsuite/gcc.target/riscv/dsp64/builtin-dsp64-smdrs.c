/* This is a test program for smdrs instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long smdrs (unsigned long ra, unsigned long rb)
{
  return __nds__smdrs (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_smdrs (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_smdrs (ra, rb);
}

int
main ()
{
  long a = smdrs (0x8000000280000002, 0x8000000180000001);
  int32x2_t va = v_smdrs ((int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
			  (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0xc0000002c0000002)
    abort ();
  else if (va[0] != 0xffffffff
	   || va[1] != 0xffffffff)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
