/* This is a test program for radd16 instruction.  */
/* { dg-do run } */
/* { dg-require-effective-target riscv32 } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#if defined(__riscv_dsp) && !defined(__riscv_32e)
static __attribute__ ((noinline))
unsigned long radd16 (unsigned long ra, unsigned long rb)
{
  return __nds__radd16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_radd16 (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_radd16 (ra, rb);
}

int
main ()
{
  unsigned long a = radd16 (0x7fff7fff, 0x7fff7fff);
  int16x2_t va = v_radd16 ((int16x2_t) {0x8000, 0x4000},
			   (int16x2_t) {0x8000, 0x8000});

  if (a != 0x7fff7fff)
    abort ();
  else if (va[0] != (short) 0x8000
	   || va[1] != (short) 0xe000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
