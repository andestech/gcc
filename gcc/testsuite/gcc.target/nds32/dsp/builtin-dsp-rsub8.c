/* This is a test program for rsub8 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int rsub8 (unsigned int ra, unsigned int rb)
{
  return __nds32__rsub8 (ra, rb);
}

static __attribute__ ((noinline))
int8x4_t v_rsub8 (int8x4_t ra, int8x4_t rb)
{
  return __nds32__v_rsub8 (ra, rb);
}

int
main ()
{
  unsigned int a = rsub8 (0x55667788, 0x11223344);
  int8x4_t va = v_rsub8 ((int8x4_t) {0x7f, 0x80, 0x80, 0xaa},
			 (int8x4_t) {0x80, 0x7f, 0x40, 0xaa});

  if (a != 0x222222a2)
    abort ();
  else if (va[0] != 0x7f
	   || va[1] != (char) 0x80
	   || va[2] != (char) 0xa0
	   || va[3] != 0)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
