/* This is a test program for sril16.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srli16_u (unsigned long ra)
{
  return __nds__srl16_u (ra, 4);
}

static __attribute__ ((noinline))
uint16x2_t v_srli16_u (uint16x2_t ra)
{
  return __nds__v_srl16_u (ra, 4);
}

int
main ()
{
  unsigned long a = srli16_u (0x0f00f000);
  uint16x2_t va = v_srli16_u ((uint16x2_t) {0x7fff, 0x8000});

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
