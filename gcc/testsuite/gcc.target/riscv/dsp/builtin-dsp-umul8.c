/* This is a test program for umul8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long umul8 (unsigned long ra, unsigned long rb)
{
  return __nds__umul8 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_umul8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds__v_umul8 (ra, rb);
}

int
main ()
{
  unsigned long long a = umul8 (0xffff0000, 0x0001ffff);
  uint16x4_t va = v_umul8 ((uint8x4_t) {0xff, 0xee, 0xdd, 0xcc},
			    (uint8x4_t) {0x11, 0x22, 0x33, 0x44});

  if (a != 0xff00000000)
    abort ();
  else if (va[0] != 0x10ef
           || va[1] != 0x1f9c
           || va[2] != 0x2c07
           || va[3] != 0x3630)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
