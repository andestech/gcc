/* This is a test program for kdmtt16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
long kdmtt16 (unsigned long ra, unsigned long rb)
{
  return __nds__kdmtt16 (ra, rb);
}

static __attribute__ ((noinline))
long v_kdmtt16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_kdmtt16 (ra, rb);
}

int
main ()
{
  long a = kdmtt16 (0x0001f0000001f000, 0x0001100000011000);
  long v_sa = v_kdmtt16 ((int16x4_t) {0xf777, 0xf111, 0xf777, 0xf111},
			 (int16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});
  if (a != 0x0000000200000002)
    abort ();
  else if (v_sa != 0xfc444000fc444000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
