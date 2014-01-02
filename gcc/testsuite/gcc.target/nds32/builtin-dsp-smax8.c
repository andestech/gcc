/* This is a test program for smax8 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int smax8 (unsigned int ra, unsigned int rb)
{
  return __nds32__smax8 (ra, rb);
}

static __attribute__ ((noinline))
int8x4_t v_smax8 (int8x4_t ra, int8x4_t rb)
{
  return __nds32__v_smax8 (ra, rb);
}


int
main ()
{
  unsigned int a = smax8 (0xffff0000, 0xfefe0001);
  int8x4_t va = v_smax8 ((int8x4_t) {0x7f, 0x7f, 0x01, 0x01},
			 (int8x4_t) {0x7e, 0x7e, 0x00, 0x00});

  if (a != 0xffff0001)
    abort ();
  else if (va[0] != 0x7f
           || va[1] != 0x7f
           || va[2] != 1
	   || va[3] != 1)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
