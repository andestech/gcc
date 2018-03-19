/* This is a test program for ucmple8 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long ucmple8 (unsigned long ra, unsigned long rb)
{
  return __nds__ucmple8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_ucmple8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds__v_ucmple8 (ra, rb);
}

int
main ()
{
  unsigned long a = ucmple8 (0xfefe0101, 0xffff0000);
  uint8x4_t va = v_ucmple8 ((uint8x4_t) {0x7e, 0x7e, 0x01, 0x01},
			    (uint8x4_t) {0x7f, 0x7f, 0x00, 0x00});

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
