/* This is a test program for sll16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int sll16 (unsigned int ra, unsigned int rb)
{
  return __nds32__sll16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_sll16 (uint16x2_t ra, unsigned int rb)
{
  return __nds32__v_sll16 (ra, rb);
}

int
main ()
{
  unsigned int a = sll16 (0x0f00f000, 4);
  uint16x2_t va = v_sll16 ((uint16x2_t) {0x7fff, 0x8000}, 4);

  if (a != 0xf0000000)
    abort ();
  else if (va[0] != 0xfff0
	   || va[1] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
