/* This is a test program for srai16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long srai16 (unsigned long ra)
{
  return __nds__sra16 (ra, 4);
}

static __attribute__ ((noinline))
int16x2_t v_srai16 (int16x2_t ra)
{
  return __nds__v_sra16 (ra, 4);
}

int
main ()
{
  unsigned long a = srai16 (0x0ffff000);

  int16x2_t aa;
  int16x2_t va = v_srai16 ((int16x2_t) {0x7fff, 0x8000});

  if (a != 0x00ffff00)
    abort ();
  else if (va[0] != 0x7ff
	   || va[1] != (short) 0xf800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
