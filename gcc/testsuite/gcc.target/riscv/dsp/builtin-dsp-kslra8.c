/* This is a test program for add8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long uclip8 (unsigned long ra)
{
  return __nds__uclip8 (ra, 2);
}

static __attribute__ ((noinline))
uint8x4_t v_uclip8 (uint8x4_t ra)
{
  return __nds__v_uclip8 (ra, 3);
}

int
main ()
{
  signed long a = uclip8 (0x10101010);
  uint8x4_t v_sa = v_uclip8 ((uint8x4_t) {1, -20, 11, 3});

  if (a != 0x03030303)
    abort ();
  else if (v_sa[0] != 1
	   || v_sa[1] != 0
	   || v_sa[2] != 7
	   || v_sa[3] != 3)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
