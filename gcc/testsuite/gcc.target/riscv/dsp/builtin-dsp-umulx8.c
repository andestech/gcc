/* This is a test program for umulx8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long umulx8 (unsigned long ra, unsigned long rb)
{
  return __nds__umulx8 (ra, rb);
}

static __attribute__ ((noinline))
uint16x4_t v_umulx8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds__v_umulx8 (ra, rb);
}

int
main ()
{
  unsigned long long a = umulx8 (0xffff0000, 0x0001ffff);
  uint16x4_t va = v_umulx8 ((uint8x4_t) {0xff, 0xee, 0xdd, 0xcc},
			    (uint8x4_t) {0x11, 0x22, 0x33, 0x44});

  if (a != 0xff000000000000)
    abort ();
  else if (va[0] != 0x21de
           || va[1] != 0xfce
           || va[2] != 0x3ab4
           || va[3] != 0x28a4)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
