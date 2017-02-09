/* This is a test program for scmple8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long scmple8 (unsigned long ra, unsigned long rb)
{
  return __nds__scmple8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_scmple8 (int8x4_t ra, int8x4_t rb)
{
  return __nds__v_scmple8 (ra, rb);
}

int
main ()
{
  unsigned long a = scmple8 (0xfefe0101, 0xffff0000);
  uint8x4_t va = v_scmple8 ((int8x4_t) {0x7e, 0x7e, 0x01, 0x01},
			    (int8x4_t) {0x7f, 0x7f, 0x00, 0x00});

  if (a != 0xffff0000)
    abort ();
  else if (va[0] != 0xff
           || va[1] != 0xff
           || va[2] != 0
	   || va[3] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
