/* This is a test program for srl8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long srl8 (unsigned long ra, unsigned long rb)
{
  return __nds__srl8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_srl8 (uint8x4_t ra, unsigned long rb)
{
  return __nds__v_srl8 (ra, rb);
}

int
main ()
{
  unsigned long a = srl8 (0x11223344, 4);
  uint8x4_t va = v_srl8 ((uint8x4_t) {0xff, 0xee, 0xdd, 0xcc}, 4);

  if (a != 0x1020304)
    abort ();
  else if (va[0] != 0xff
	   || va[1] != 0xfe
	   || va[2] != 0xfd
	   || va[3] != 0xfc)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
