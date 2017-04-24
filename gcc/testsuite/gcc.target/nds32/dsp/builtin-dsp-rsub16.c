/* This is a test program for rsub16 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int rsub16 (unsigned int ra, unsigned int rb)
{
  return __nds32__rsub16 (ra, rb);
}

static __attribute__ ((noinline))
int16x2_t v_rsub16 (int16x2_t ra, int16x2_t rb)
{
  return __nds32__v_rsub16 (ra, rb);
}

int
main ()
{
  unsigned int a = rsub16 (0x7fff7fff, 0x80008000);
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
