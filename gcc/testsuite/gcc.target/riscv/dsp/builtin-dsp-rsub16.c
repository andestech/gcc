/* This is a test program for rsub16 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long rsub16 (unsigned long ra, unsigned long rb)
{
  return __nds__rsub16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_rsub16 (int16x2_t ra, int16x2_t rb)
{
  return __nds__v_rsub16 (ra, rb);
}

int
main ()
{
  unsigned long a = rsub16 (0x7fff7fff, 0x80008000);
  int16x2_t va = v_rsub16 ((int16x2_t) {0x8000, 0x8000},
			   (int16x2_t) {0x7fff, 0x4000});

  if (a != 0x7fff7fff)
    abort ();
  else if (va[0] != (short) 0x8000
	   || va[1] != (short) 0xa000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
