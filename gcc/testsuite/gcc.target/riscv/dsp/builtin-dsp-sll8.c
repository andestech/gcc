/* This is a test program for sll8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sll8 (unsigned long ra, unsigned long rb)
{
  return __nds__sll8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_sll8 (uint8x4_t ra, unsigned long rb)
{
  return __nds__v_sll8 (ra, rb);
}

int
main ()
{
  unsigned long a = sll8 (0x11223344, 4);
  uint8x4_t va = v_sll8 ((uint8x4_t) {0xff, 0xee, 0xdd, 0xcc}, 4);

  if (a != 0x10203040)
    abort ();
  else if (va[0] != 0xf0
	   || va[1] != 0xe0
	   || va[2] != 0xd0
	   || va[3] != 0xc0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
