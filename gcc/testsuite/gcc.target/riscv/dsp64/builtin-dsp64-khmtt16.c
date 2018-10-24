/* This is a test program for khmtt16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
long khmtt16 (unsigned long ra, unsigned long rb)
{
  return __builtin_riscv_khmtt16 (ra, rb);
}

static __attribute__ ((noinline))
long v_khmtt16 (int16x4_t ra, int16x4_t rb)
{
  return __nds__v_khmtt16 (ra, rb);
}

int
main ()
{
  long a = khmtt16 (0x0001f0000001f000, 0x0001100000011000);
  long v_sa = v_khmtt16 ((int16x4_t) {0xf777, 0xf111, 0xf777, 0xf111},
		       (int16x4_t) {0x1000, 0x2000, 0x1000, 0x2000});
  if (a != 0)
    abort ();
  else if (v_sa != 0xfffffc44fffffc44)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
