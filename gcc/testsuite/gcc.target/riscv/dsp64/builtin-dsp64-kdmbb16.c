/* This is a test program for kdmbb16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long kdmbb16 (unsigned long ra, unsigned long rb)
{
  return __nds__kdmbb16 (ra, rb);
}

static __attribute__ ((noinline))
long v_kdmbb16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kdmbb16 (ra, rb);
}

int
main ()
{
  long a = kdmbb16 (0x0001f0000001f000, 0x0001100000011000);
  long v_sa = v_kdmbb16 ((int16x4_t) {0xf777, 0xf111, 0xf777, 0xf111},
			 (int16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});
  if (a != 0xfe000000fe000000)
    abort ();
  else if (v_sa != 0xfeeee000feeee000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
