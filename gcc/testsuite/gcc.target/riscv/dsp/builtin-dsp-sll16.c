/* This is a test program for sll16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sll16 (unsigned long ra, unsigned long rb)
{
  return __nds__sll16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_sll16 (uint16x2_t ra, unsigned long rb)
{
  return __nds__v_sll16 (ra, rb);
}

int
main ()
{
  unsigned long a = sll16 (0x0f00f000, 4);
  uint16x2_t va = v_sll16 ((uint16x2_t) {0x7fff, 0x8000}, 4);

  if (a != 0xf0000000)
    abort ();
  else if (va[0] != 0xfff0
	   || va[1] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
