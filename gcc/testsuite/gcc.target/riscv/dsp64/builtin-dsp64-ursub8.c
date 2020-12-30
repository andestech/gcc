/* This is a test program for ursub8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long ursub8 (unsigned long ra, unsigned long rb)
{
  return __nds__ursub8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_ursub8 (uint8x8_t ra, uint8x8_t rb)
{
  return __nds__v_ursub8 (ra, rb);
}

int
main ()
{
  unsigned long a = ursub8 (0x5566778855667788, 0x1122334411223344);
  uint8x8_t va = v_ursub8 ((uint8x8_t) {0x7f, 0x80, 0x80, 0xaa, 0x7f, 0x80, 0x80, 0xaa},
			   (uint8x8_t) {0x80, 0x7f, 0x40, 0xaa, 0x80, 0x7f, 0x40, 0xaa});

  if (a != 0x2222222222222222)
    abort ();
  else if (va[0] != 0xff
	   || va[1] != 0
	   || va[2] != 0x20
	   || va[3] != 0
	   || va[4] != 0xff
	   || va[5] != 0
	   || va[6] != 0x20
	   || va[7] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
