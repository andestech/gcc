/* This is a test program for khmbt instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long khmbt (int ra, int rb)
{
  return __nds__khmbt (ra, rb);
}

static __attribute__ ((noinline))
long v_khmbt (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_khmbt (ra, rb);
}

int
main ()
{
  long a = khmbt (0x0001f000, 0x00011000);
  long v_sa = v_khmbt ((int16x4_t) {0xf777, 0xf111, 0, 0},
		       (int16x4_t) {0x1000, 0x2000, 0, 0});
  if (a != 0xffffffffffffffff)
    abort ();
  else if (v_sa != 0xfffffffffffffddd)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
