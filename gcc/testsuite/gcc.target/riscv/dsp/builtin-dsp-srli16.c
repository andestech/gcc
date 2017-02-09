/* This is a test program for srli16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srli16 (unsigned long ra)
{
  return __nds__srl16 (ra, 4);
}

static __attribute__ ((noinline))
uint16x2_t v_srli16 (uint16x2_t ra)
{
  return __nds__v_srl16 (ra, 4);
}

int
main ()
{
  unsigned long a = srli16 (0x0f00f000);
  uint16x2_t va = v_srli16 ((uint16x2_t) {0x7fff, 0x8000});

  if (a != 0xf00f00)
    abort ();
  else if (va[0] != 0x7ff
	   || va[1] != 0x0800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
