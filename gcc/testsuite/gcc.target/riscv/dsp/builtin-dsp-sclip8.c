/* This is a test program for add8 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
signed long sclip8 (signed long ra)
{
  return __nds__sclip8 (ra, 2);
}

static __attribute__ ((noinline))
int8x4_t v_sclip8 (int8x4_t ra)
{
  return __nds__v_sclip8 (ra, 3);
}

int
main ()
{
  signed long a = sclip8 (0x10101010);
  int8x4_t v_sa = v_sclip8 ((int8x4_t) {0, -20, 11, 3});

  if (a != 0x03030303)
    abort ();
  else if (v_sa[0] != 0
	   || v_sa[1] != -8
	   || v_sa[2] != 7
	   || v_sa[3] != 3)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
