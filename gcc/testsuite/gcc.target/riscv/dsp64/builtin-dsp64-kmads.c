/* This is a test program for kmads instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kmads (unsigned long rd, unsigned long ra, unsigned long rb)
{
  return __nds__kmads (rd, ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kmads (int32x2_t rd, int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kmads (rd, ra, rb);
}

int
main ()
{
  long a = kmads (0x300000003, 0x8000000280000002, 0x8000000180000001);
  int32x2_t va = v_kmads ((int32x2_t) {0x3, 0x3},
		     (int16x4_t) {0xffff, 0x0002, 0xffff, 0x0002},
		     (int16x4_t) {0xffff, 0x0001, 0xffff, 0x0001});

  if (a != 0x4000000140000001)
    abort ();
  else if (va[0] != 0x4
	   || va[1] != 0x4)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
