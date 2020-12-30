/* This is a test program for uradd8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long uradd8 (unsigned long ra, unsigned long rb)
{
  return __nds__uradd8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x8_t v_uradd8 (uint8x8_t ra, uint8x8_t rb)
{
  return __nds__v_uradd8 (ra, rb);
}

int
main ()
{
  unsigned long a = uradd8 (0x1122334411223344, 0x5566778855667788);
  uint8x8_t va = v_uradd8 ((uint8x8_t) {0x7f, 0x80, 0x40, 0xaa, 0x7f, 0x80, 0x40, 0xaa},
			   (uint8x8_t) {0x7f, 0x80, 0x80, 0xaa, 0x7f, 0x80, 0x80, 0xaa});

  if (a != 0x3344556633445566)
    abort ();
  else if (va[0] != 0x7f
	   || va[1] != 0x80
	   || va[2] != 0x60
	   || va[3] != 0xaa
	   || va[4] != 0x7f
	   || va[5] != 0x80
	   || va[6] != 0x60
	   || va[7] != 0xaa)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
