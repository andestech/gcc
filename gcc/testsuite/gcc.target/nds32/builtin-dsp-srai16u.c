/* This is a test program for srai16.u instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int srai16u (unsigned int ra)
{
  return __nds32__sra16_u (ra, 4);
}

static __attribute__ ((noinline))
int16x2_t v_srai16u (int16x2_t ra)
{
  return __nds32__v_sra16_u (ra, 4);
}

int
main ()
{
  unsigned int a = srai16u (0x0ffff000);
  int16x2_t va = v_srai16u ((int16x2_t) {0x7fff, 0x8000});

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
