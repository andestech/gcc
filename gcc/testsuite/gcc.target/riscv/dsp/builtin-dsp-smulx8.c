/* This is a test program for smulx8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long long smulx8 (unsigned long ra, unsigned long rb)
{
  return __nds__smulx8 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_smulx8 (int8x4_t ra, int8x4_t rb)
{
  return __nds__v_smulx8 (ra, rb);
}

int
main ()
{
  unsigned long long a = smulx8 (0xffff0000, 0x0001ffff);
  int16x4_t va = v_smulx8 ((int8x4_t) {0xff, 0xee, 0xdd, 0xcc},
			   (int8x4_t) {0x11, 0x22, 0x33, 0x44});

  if (a != 0xffff000000000000)
    abort ();
  else if (va[0] != (short) 0xffde
           || va[1] != (short) 0xfece
           || va[2] != (short) 0xf6b4
           || va[3] != (short) 0xf5a4)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
