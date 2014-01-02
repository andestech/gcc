/* This is a test program for umax8 instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
static __attribute__ ((noinline))
unsigned int umax8 (unsigned int ra, unsigned int rb)
{
  return __nds32__umax8 (ra, rb);
}

static __attribute__ ((noinline))
uint8x4_t v_umax8 (uint8x4_t ra, uint8x4_t rb)
{
  return __nds32__v_umax8 (ra, rb);
}


int
main ()
{
  unsigned int a = umax8 (0xffff0000, 0xfffe0001);
  uint8x4_t va = v_umax8 ((uint8x4_t) {0xff, 0xff, 0x01, 0x01},
			  (uint8x4_t) {0xfe, 0xfe, 0x00, 0x00});

  if (a != 0xffff0001)
    abort ();
  else if (va[0] != 0xff
           || va[1] != 0xff
           || va[2] != 1
	   || va[3] != 1)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
