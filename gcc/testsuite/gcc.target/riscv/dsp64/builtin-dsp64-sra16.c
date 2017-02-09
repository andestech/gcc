/* This is a test program for sra16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv64 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long sra16 (unsigned long ra, unsigned long rb)
{
  return __nds__sra16 (ra, rb);
}

static __attribute__ ((noinline))
int16x4_t v_sra16 (int16x4_t ra, unsigned long rb)
{
  return __nds__v_sra16 (ra, rb);
}

int
main ()
{
  unsigned long a = sra16 (0x0ffff0000ffff000, 4);
  int16x4_t va = v_sra16 ((int16x4_t) {0x7fff, 0x8000, 0x7fff, 0x8000}, 4);

  if (a != 0x00ffff0000ffff00)
    abort ();
  else if (va[0] != 0x7ff
	   || va[1] != (short) 0xf800
	   || va[2] != 0x7ff
	   || va[3] != (short) 0xf800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
