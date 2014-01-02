/* This is a test program for ursub16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int ursub16 (unsigned int ra, unsigned int rb)
{
  return __nds32__ursub16 (ra, rb);
}

static __attribute__ ((noinline))
uint16x2_t v_ursub16 (uint16x2_t ra, uint16x2_t rb)
{
  return __nds32__v_ursub16 (ra, rb);
}

int
main ()
{
  unsigned int a = ursub16 (0x7fff7fff, 0x80008000);
  uint16x2_t va = v_ursub16 ((uint16x2_t) {0x8000, 0x8000},
			     (uint16x2_t) {0x7fff, 0x4000});

  if (a != 0xffffffff)
    abort ();
  else if (va[0] != 0
	   || va[1] != 0x2000)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
