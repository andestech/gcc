/* This is a test program for srai16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int srai16 (unsigned int ra)
{
  return __nds32__sra16 (ra, 4);
}

static __attribute__ ((noinline))
int16x2_t v_srai16 (int16x2_t ra)
{
  return __nds32__v_sra16 (ra, 4);
}

int
main ()
{
  unsigned int a = srai16 (0x0ffff000);

  int16x2_t aa;
  int16x2_t va = v_srai16 ((int16x2_t) {0x7fff, 0x8000});

  if (a != 0x00ffff00)
    abort ();
  else if (va[0] != 0x7ff
	   || va[1] != (short) 0xf800)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
