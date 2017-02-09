/* This is a test program for radd8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long radd8 (unsigned long ra, unsigned long rb)
{
  return __nds__radd8 (ra, rb);
}

static __attribute__ ((noinline))
int8x8_t v_radd8 (int8x8_t ra, int8x8_t rb)
{
  return __nds__v_radd8 (ra, rb);
}

int
main ()
{
  unsigned long a = radd8 (0x1122334411223344, 0x5566778855667788);
  int8x8_t va = v_radd8 ((int8x8_t) {0x7f, 0x80, 0x80, 0xaa, 0x7f, 0x80, 0x80, 0xaa},
			 (int8x8_t) {0x7f, 0x80, 0x40, 0xaa, 0x7f, 0x80, 0x40, 0xaa});

  if (a != 0x334455e6334455e6)
    abort ();
  else if (va[0] != 0x7f
	   || va[1] != (signed char) 0x80
	   || va[2] != (signed char) 0xe0
	   || va[3] != (signed char) 0xaa
	   || va[4] != 0x7f
	   || va[5] != (signed char) 0x80
	   || va[6] != (signed char) 0xe0
	   || va[7] != (signed char) 0xaa)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
