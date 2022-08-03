/* This is a test program for uradd16 instruction.  */

/* { dg-do run } */

#include <nds_intrinsic.h>
#include <stdlib.h>

#ifdef __riscv_dsp
static __attribute__ ((noinline))
unsigned long uradd16 (unsigned long ra, unsigned long rb)
{
  return __nds__uradd16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_uradd16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds__v_uradd16 (ra, rb);
}

int
main ()
{
  unsigned long a = uradd16 (0x7fff7fff, 0x7fff7fff);
  uint16x2_t va = v_uradd16 ((uint16x2_t) {0x8000, 0x4000},
			     (uint16x2_t) {0x8000, 0x8000});

  if (a != 0x7fff7fff)
    abort ();
  else if (va[0] != 0x8000
	   || va[1] != 0x6000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
