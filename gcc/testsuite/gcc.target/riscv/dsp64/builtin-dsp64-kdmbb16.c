/* This is a test program for kdmbb16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long kdmbb16 (unsigned long ra, unsigned long rb)
{
  return __nds__kdmbb16 (ra, rb);
}

static __attribute__ ((noinline))
int32x2_t v_kdmbb16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kdmbb16 (ra, rb);
}

int
main ()
{
  unsigned long a = kdmbb16 (0x0001f0000001f000, 0x0001100000011000);
  int32x2_t va = v_kdmbb16 ((int16x4_t) {0xf777, 0xf111, 0xf777, 0xf111},
			    (int16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});
  if (a != 0xfe000000fe000000)
    abort ();
  else if (va[0] != 0xfeeee000
	   || va[1] != 0xfeeee000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
