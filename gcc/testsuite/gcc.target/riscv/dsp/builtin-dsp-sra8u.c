/* This is a test program for sra8.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sra8u (unsigned long ra, unsigned long rb)
{
  return __nds__sra8_u (ra, rb);
}

static __attribute__ ((noinline))
int8x4_t v_sra8u (int8x4_t ra, unsigned long rb)
{
  return __nds__v_sra8_u (ra, rb);
}

int
main ()
{
  unsigned long a = sra8u (0x011223344, 4);
  int8x4_t va = v_sra8u ((int8x4_t) {0xff, 0xee, 0xdd, 0xcc}, 4);

  if (a != 0x1020304)
    abort ();
  else if (va[0] != 0
	   || va[1] != (signed char) 0xff
	   || va[2] != (signed char) 0xfe
	   || va[3] != (signed char) 0xfd)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
