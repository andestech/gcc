/* This is a test program for uradd16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int uradd16 (unsigned int ra, unsigned int rb)
{
  return __nds32__uradd16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_uradd16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_uradd16 (ra, rb);
}

int
main ()
{
  unsigned int a = uradd16 (0x7fff7fff, 0x7fff7fff);
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
