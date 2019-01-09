/* This is a test program for sra16.u instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sra16u (unsigned long ra, unsigned long rb)
{
  return __nds__sra16_u (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_sra16u (int16x2_t ra, unsigned long rb)
{
  return __nds__v_sra16_u (ra, rb);
}

int
main ()
{
  unsigned long a = sra16u (0x0ffff000, 4);
  int16x2_t va = v_sra16u ((int16x2_t) {0x7fff, 0x8000}, 4);

  if (a != 0x100ff00)
    abort ();
  else if (va[0] != 0x800
	   || va[1] != (short) 0xf800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
