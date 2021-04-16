/* This is a test program for srl16.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srl16_u (unsigned long ra, unsigned long rb)
{
  return __nds__srl16_u (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_srl16_u (uint16x2_t ra, unsigned long rb)
{
  return __nds__v_srl16_u (ra, rb);
}

int
main ()
{
  unsigned long a = srl16_u (0x0f00f000, 4);
  uint16x2_t va = v_srl16_u ((uint16x2_t) {0x7fff, 0x8000}, 4);

  if (a != 0xf00f00)
    abort ();
  else if (va[0] != 0x800
	   || va[1] != 0x800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
