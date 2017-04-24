/* This is a test program for sril16.u instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int srli16_u (unsigned int ra)
{
  return __nds32__srl16_u (ra, 4);
}

static __attribute__ ((noinline))
uint16x2_t v_srli16_u (uint16x2_t ra)
{
  return __nds32__v_srl16_u (ra, 4);
}

int
main ()
{
  unsigned int a = srli16_u (0x0f00f000);
  uint16x2_t va = v_srli16_u ((uint16x2_t) {0x7fff, 0x8000});

  if (a != 0xf00f00)
    abort ();
  else if (va[0] != 0x800
	   || va[1] != 0x800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
